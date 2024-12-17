#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <unordered_map>

#include "Debug.h"
#include "Flag.h"
#include "Vector.h"

namespace /* Tools */ {
#define EVENT_SIZE    (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))
constexpr Uint DEFAULT_MASK = (IN_ACCESS | IN_MODIFY | IN_ATTRIB | IN_CLOSE | IN_OPEN | IN_MOVE | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF);
#define DEFAULT_MASK DEFAULT_MASK
#define CALLBACK      void (*callback)(const char *file_path, inotify_event *event)
}

struct FileEvent {
  const char *file_path;
  inotify_event *e;
  CALLBACK;
};

class FileListener {
 private:
  int                   fd;
  int                   wd;
  pthread_t             thread;
  std::queue<FileEvent> event_queue;
  pthread_mutex_t       queue_mutex;
  pthread_cond_t        queue_cond;
  bool                  running;

  static void *event_handler(void *arg) {
    FileListener *listener = (FileListener *)arg;
    while (listener->running) {
      pthread_mutex_lock(&listener->queue_mutex);
      while (listener->event_queue.empty() && listener->running) {
        pthread_cond_wait(&listener->queue_cond, &listener->queue_mutex);
      }
      if (!listener->running) {
        pthread_mutex_unlock(&listener->queue_mutex);
        break;
      }
      FileEvent event = listener->event_queue.front();
      listener->event_queue.pop();
      pthread_mutex_unlock(&listener->queue_mutex);
      /* Call the user-defined callback. */
      event.callback(event.file_path, event.e);
    }
    return NULL;
  }

 public:
  FileListener(void) {
    fd = -1;
    wd = -1;
    running = TRUE;
    pthread_mutex_init(&queue_mutex, NULL);
    pthread_cond_init(&queue_cond, NULL);
  }

  ~FileListener(void) {
    stop();
  }

  int listen(const char *file_path, CALLBACK, Uint flags = DEFAULT_MASK) noexcept {
    fd = inotify_init();
    if (fd < 0) {
      logE("File: %s: inotify_init() Failed. errno: %s", file_path, strerror(errno));
      return -1;
    }
    wd = inotify_add_watch(fd, file_path, flags);
    if (wd < 0) {
      logE("File: %s: inotify_add_watch() Failed. errno: %s", file_path, strerror(errno));
      close(fd);
      return -1;
    }
    if (pthread_create(&thread, NULL, event_handler, this) != 0) {
      logE("File: %s: Failed to create event handler thread.", file_path);
      close(fd);
      return -1;
    }
    char buffer[EVENT_BUF_LEN];
    while (running) {
      long length = read(fd, buffer, EVENT_BUF_LEN);
      if (length < 0) {
        logE("FileListener: read() Failed. errno: %s", strerror(errno));
        break;
      }
      for (long i = 0; i < length;) {
        struct inotify_event *event = (struct inotify_event *)&buffer[i];
        queue_event(file_path, event, callback);
        /* File is gone, exit loop. */
        if (event->mask == IN_DELETE) {
          break;
        }
        i += (EVENT_SIZE + event->len);
      }
    }
    if (fd >= 0) {
      inotify_rm_watch(fd, wd);
      close(fd);
      fd = -1;
    }
    return 0;
  }

  void queue_event(const char *file_path, inotify_event *e, CALLBACK) {
    pthread_mutex_lock(&queue_mutex);
    event_queue.emplace(file_path, e, callback);
    pthread_cond_signal(&queue_cond);
    pthread_mutex_unlock(&queue_mutex);
  }

  void stop(void) {
    if (!running) {
      return;
    }
    running = FALSE;
    pthread_mutex_lock(&queue_mutex);
    pthread_cond_broadcast(&queue_cond);
    pthread_mutex_unlock(&queue_mutex);
    if (fd >= 0) {
      inotify_rm_watch(fd, wd);
      close(fd);
      fd = -1;
    }
    if (thread) {
      pthread_join(thread, NULL);
    }
    pthread_mutex_destroy(&queue_mutex);
    pthread_cond_destroy(&queue_cond);
  }
};

class FileListenerHandler {
 private:
  struct FileListenerThread {
    FileListener listener;
    pthread_t    thread;
    const char *file;
    Uint mask;
    CALLBACK;
  };
  std::unordered_map<std::string, FileListenerThread *> _listeners;

  static void *_handler(void *arg) {
    FileListenerThread *data = (FileListenerThread *)arg;
    data->listener.listen(data->file, data->callback, data->mask);
    return NULL;
  }

 public:
  FileListenerHandler(void) {}

  ~FileListenerHandler(void) {
    stop_all();
  }

  void add_listener(const char *file_path, CALLBACK, Uint mask = DEFAULT_MASK) noexcept {
    if (_listeners.count(file_path)) {
      return;
    }
    FileListenerThread *data = new FileListenerThread();
    data->file = file_path;
    data->mask = mask;
    data->callback = callback;
    _listeners[file_path] = data;
    pthread_create(&data->thread, NULL, _handler, data);
  }
  
  void stop_listener(const char *path) {
    if (_listeners.count(path)) {
      _listeners[path]->listener.stop();
      pthread_join(_listeners[path]->thread, NULL);
      delete _listeners[path];
      _listeners.erase(path);
    }
  }

  void stop_all(void) {
    for (auto &[file, data] : _listeners) {
      data->listener.stop();
      pthread_join(data->thread, NULL);
      delete data;
    }
    _listeners.clear();
  }
};

typedef enum {
  FILE_LISTENER_RUNNING,
  #define FILE_LISTENER_RUNNING FILE_LISTENER_RUNNING
} file_listener_flag_t;

typedef struct file_listener_t {
  DELETE_COPY_AND_MOVE_CONSTRUCTORS(file_listener_t);

 private:
  char            *_file_path;
  int              _fd;
  int              _wd;
  pthread_t        _callback_thread;
  pthread_t        _listener_thread;
  pthread_mutex_t  _mutex;
  pthread_cond_t   _cond;
  std::queue<Uint> _event_queue;
  bit_flag_t<8>    _flag;
  /* Callbacks. */
  void (*_access_callback)(void *);
  void (*_modify_callback)(void *);
  void (*_attrib_callback)(void *);
  void (*_close_write_callback)(void *);
  void (*_close_nowrite_callback)(void *);
  void (*_open_callback)(void *);
  void (*_moved_from_callback)(void *);
  void (*_moved_to_callback)(void *);
  void (*_create_callback)(void *);
  void (*_delete_callback)(void *);
  void (*_delete_self_callback)(void *);
  void (*_move_self_callback)(void *);
  /* Callback data. */
  void *_access_callback_data;
  void *_modify_callback_data;
  void *_attrib_callback_data;
  void *_close_write_callback_data;
  void *_close_nowrite_callback_data;
  void *_open_callback_data;
  void *_moved_from_callback_data;
  void *_moved_to_callback_data;
  void *_create_callback_data;
  void *_delete_callback_data;
  void *_delete_self_callback_data;
  void *_move_self_callback_data;
  
  static void *_callback_loop(void *arg) {
    file_listener_t *self = (file_listener_t *)arg;
    while (self->_flag.is_set<FILE_LISTENER_RUNNING>()) {
      pthread_mutex_lock(&self->_mutex);
      while (self->_event_queue.empty() && self->_flag.is_set<FILE_LISTENER_RUNNING>()) {
        pthread_cond_wait(&self->_cond, &self->_mutex);
      }
      if (!self->_flag.is_set<FILE_LISTENER_RUNNING>()) {
        pthread_mutex_unlock(&self->_mutex);
        break;
      }
      Uint mask = self->_event_queue.front();
      self->_event_queue.pop();
      pthread_mutex_unlock(&self->_mutex);
      if (mask & IN_ACCESS) {
        self->_access_callback(self->_access_callback_data);
      }
      if (mask & IN_MODIFY) {
        self->_modify_callback(self->_modify_callback_data);
      }
      if (mask & IN_ATTRIB) {
        self->_attrib_callback(self->_attrib_callback_data);
      }
      if (mask & IN_CLOSE_WRITE) {
        self->_close_write_callback(self->_close_write_callback_data);
      }
      if (mask & IN_CLOSE_NOWRITE) {
        self->_close_nowrite_callback(self->_close_nowrite_callback_data);
      }
      if (mask & IN_OPEN) {
        self->_open_callback(self->_open_callback_data);
      }
      if (mask & IN_MOVED_FROM) {
        self->_moved_from_callback(self->_moved_from_callback_data);
      }
      if (mask & IN_MOVED_TO) {
        self->_moved_to_callback(self->_moved_to_callback_data);
      }
      if (mask & IN_CREATE) {
        self->_create_callback(self->_create_callback_data);
      }
      if (mask & IN_DELETE) {
        self->_delete_callback(self->_delete_callback_data);
      }
      if (mask & IN_DELETE_SELF) {
        self->_delete_self_callback(self->_delete_self_callback_data);
      }
      if (mask & IN_MOVE_SELF) {
        self->_move_self_callback(self->_move_self_callback_data);
      }
    }
    return NULL;
  }

  static void *_listener_loop(void *arg) {
    file_listener_t *self = (file_listener_t *)arg;
    char buffer[EVENT_BUF_LEN];
    while (self->_flag.is_set<FILE_LISTENER_RUNNING>()) {
      long length = read(self->_fd, buffer, EVENT_BUF_LEN);
      if (length < 0) {
        logE("fd: '%s': 'read()' failed.", self->_file_path);
        break;
      }
      for (long i = 0; i < length;) {
        inotify_event *event = (inotify_event *)&buffer[i];
        /* Emplace the event in the queue. */
        pthread_mutex_lock(&self->_mutex);
        self->_event_queue.emplace(event->mask);
        pthread_cond_signal(&self->_cond);
        pthread_mutex_unlock(&self->_mutex);
        i += (EVENT_SIZE + event->len);
      }
    }
    return NULL;
  }

 public:
  /* If 'start_listening()' is not called we will leak memory. */
  file_listener_t(const char *file) {
    _fd = -1;
    _wd = -1;
    _callback_thread = 0;
    _listener_thread = 0;
    _flag.clear();
    Ulong file_len = strlen(file);
    _file_path = (char *)malloc(file_len + 1);
    memcpy(_file_path, file, file_len);
    _file_path[file_len] = '\0';
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_cond, NULL);
    _access_callback        = [](void *) {};
    _modify_callback        = [](void *) {};
    _attrib_callback        = [](void *) {};
    _close_write_callback   = [](void *) {};
    _close_nowrite_callback = [](void *) {};
    _open_callback          = [](void *) {};
    _moved_from_callback    = [](void *) {};
    _moved_to_callback      = [](void *) {};
    _create_callback        = [](void *) {};
    _delete_callback        = [](void *) {};
    _delete_self_callback   = [](void *) {};
    _move_self_callback     = [](void *) {};
    _access_callback_data        = NULL;
    _modify_callback_data        = NULL;
    _attrib_callback_data        = NULL;
    _close_write_callback_data   = NULL;
    _close_nowrite_callback_data = NULL;
    _open_callback_data          = NULL;
    _moved_from_callback_data    = NULL;
    _moved_to_callback_data      = NULL;
    _create_callback_data        = NULL;
    _delete_callback_data        = NULL;
    _delete_self_callback_data   = NULL;
    _move_self_callback_data     = NULL;
  }

  ~file_listener_t(void) {
    stop();
  }

  void stop(void) {
    if (!_flag.is_set<FILE_LISTENER_RUNNING>()) {
      return;
    }
    _flag.unset<FILE_LISTENER_RUNNING>();
    pthread_mutex_lock(&_mutex);
    pthread_cond_broadcast(&_cond);
    pthread_mutex_unlock(&_mutex);
    if (_fd >= 0) {
      inotify_rm_watch(_fd, _wd);
      close(_fd);
      _fd = -1;
    }
    if (_callback_thread) {
      pthread_join(_callback_thread, NULL);
      _callback_thread = 0;
    }
    if (_listener_thread) {
      pthread_join(_listener_thread, NULL);
      _listener_thread = 0;
    }
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_cond);
    free(_file_path);
    _file_path = NULL;
  }

  int start_listening(Uint mask = DEFAULT_MASK) {
    _flag.set<FILE_LISTENER_RUNNING>();
    _fd = inotify_init();
    if (_fd < 0) {
      logE("fd: '%s': inotify_init() Failed. errno: %s", _file_path, strerror(errno));
      return -1;
    }
    _wd = inotify_add_watch(_fd, _file_path, mask);
    if (_wd < 0) {
      logE("fd: '%s': inotify_add_watch() Failed. errno: %s", _file_path, strerror(errno));
      close(_fd);
      return -1;
    }
    if (pthread_create(&_callback_thread, NULL, _callback_loop, this) != 0) {
      logE("fd: '%s': Failed to create callback thread.", _file_path);
      stop();
      return -1;
    }
    if (pthread_create(&_listener_thread, NULL, _listener_loop, this) != 0) {
      logE("fd: '%s': Failed to create listener thread.", _file_path);
      stop();
      return -1;
    }
    return 0;
  }

  void set_event_callback(Uint mask, void *arg, void (*callback)(void *)) {
    if (_flag.is_set<FILE_LISTENER_RUNNING>()) {
      logE("Event callbacks must be set before listener is started.");
      return;
    }
    if (mask & IN_ACCESS) {
      _access_callback = callback;
      _access_callback_data = arg;
    }
    if (mask & IN_MODIFY) {
      _modify_callback = callback;
      _modify_callback_data = arg;
    }
    if (mask & IN_ATTRIB) {
      _attrib_callback = callback;
      _attrib_callback_data = arg;
    }
    if (mask & IN_CLOSE_WRITE) {
      _close_write_callback = callback;
      _close_write_callback_data = arg;
    }
    if (mask & IN_CLOSE_NOWRITE) {
      _close_nowrite_callback = callback;
      _close_nowrite_callback_data = arg;
    }
    if (mask & IN_OPEN) {
      _open_callback = callback;
      _open_callback_data = arg;
    }
    if (mask & IN_MOVED_FROM) {
      _moved_from_callback = callback;
      _moved_from_callback_data = arg;
    }
    if (mask & IN_MOVED_TO) {
      _moved_to_callback = callback;
      _moved_to_callback_data = arg;
    }
    if (mask & IN_CREATE) {
      _create_callback = callback;
      _create_callback_data = arg;
    }
    if (mask & IN_DELETE) {
      _delete_callback = callback;
      _delete_callback_data = arg;
    }
    if (mask & IN_DELETE_SELF) {
      _delete_self_callback = callback;
      _delete_self_callback_data = arg;
    }
    if (mask & IN_MOVE_SELF) {
      _move_self_callback = callback;
      _move_self_callback_data = arg;
    }
  }

  const char *const &get_file_path(void) const {
    return _file_path;
  }
} file_listener_t;

static file_listener_t *make_file_listener(const char *file_path) {
  file_listener_t *listener = (file_listener_t *)malloc(sizeof(*listener));
  new (listener) file_listener_t(file_path);
  return listener;
}

static void free_file_listener(file_listener_t *listener) {
  listener->~file_listener_t();
  free(listener);
}

typedef struct file_listener_handler_t {
  DELETE_COPY_AND_MOVE_CONSTRUCTORS(file_listener_handler_t);

 private:
  MVector<file_listener_t *> _data;

 public:
  file_listener_handler_t(void) {}

  ~file_listener_handler_t(void) {
    stop_all();
  }

  /* Do not add a listener without staring it after callbacks have been set.  Otherwise memory will leak */
  file_listener_t *const &add_listener(const char *file_path) {
    return _data.push_back(make_file_listener(file_path)).back();
  }

  /* If listener has been added but not started, this will leak memory. */
  void stop_listener(const char *file_path) {
    file_listener_t *listener = get_listener(file_path);
    free_file_listener(listener);
    _data.erase(listener);
  }

  void stop_all(void) {
    for (Uint i = 0; i < _data.size(); ++i) {
      free_file_listener(_data[i]);
    }
    _data.resize(0);
  }

  file_listener_t *get_listener(const char *file_path) {
    for (const auto &listener : _data) {
      if (strcmp(listener->get_file_path(), file_path) == 0) {
        return listener;
      }
    }
    return NULL;
  }
} file_listener_handler_t;

namespace /* Undefs */ {
#undef EVENT_SIZE
#undef EVENT_BUF_LEN
#undef DEFAULT_MASK
}

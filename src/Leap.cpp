#include "mruby.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/variable.h"
#include "Leap.h"
#include <utility>
#include <memory>
#if defined(_MSC_VER)
#include <windows.h>
#else
#include <unistd.h>
#endif

static struct RClass *top_module       = NULL;
static struct RClass *class_controller = NULL;
static struct RClass *class_listener   = NULL;
static struct RClass *class_config     = NULL;
static struct RClass *class_devicelist = NULL;

extern "C" {
static void mrb_leapmotion_controller_free(mrb_state *mrb, void *ptr);
static void mrb_leapmotion_listener_free(mrb_state *mrb, void *ptr);
static void mrb_leapmotion_vector_free(mrb_state *mrb, void *ptr);
static void mrb_leapmotion_frame_free(mrb_state *mrb, void *ptr);
static void mrb_leapmotion_handlist_free(mrb_state *mrb, void *ptr);
static void mrb_leapmotion_hand_free(mrb_state *mrb, void *ptr);
static void mrb_leapmotion_pointable_free(mrb_state *mrb, void *ptr);
static void mrb_leapmotion_fingerlist_free(mrb_state *mrb, void *ptr);
static void mrb_leapmotion_finger_free(mrb_state *mrb, void *ptr);
}

static struct mrb_data_type mrb_leapmotion_controller_type = { "Controller", mrb_leapmotion_controller_free };
static struct mrb_data_type mrb_leapmotion_listener_type   = { "Listener",   mrb_leapmotion_listener_free };
static struct mrb_data_type mrb_leapmotion_vector_type     = { "Vector",     mrb_leapmotion_vector_free };
static struct mrb_data_type mrb_leapmotion_frame_type      = { "Frame",      mrb_leapmotion_frame_free };
static struct mrb_data_type mrb_leapmotion_handlist_type   = { "HandList",   mrb_leapmotion_handlist_free };
static struct mrb_data_type mrb_leapmotion_hand_type       = { "Hand",       mrb_leapmotion_hand_free };
static struct mrb_data_type mrb_leapmotion_pointable_type  = { "Pointable",  mrb_leapmotion_pointable_free };
static struct mrb_data_type mrb_leapmotion_fingerlist_type = { "FingerList", mrb_leapmotion_fingerlist_free };
static struct mrb_data_type mrb_leapmotion_finger_type     = { "Finger",     mrb_leapmotion_finger_free };

/*
 * mruby's warapped native data types.
 */
typedef struct mrb_leapmotion_controller_t_ {
  Leap::Controller *controller;
} mrb_leapmotion_controller_t;

typedef struct mrb_leapmotion_listener_t_ {
  Leap::Listener *listener;
} mrb_leapmotion_listener_t;

typedef struct mrb_leapmotion_vector_t_ {
  Leap::Vector *obj;
} mrb_leapmotion_vector_t;

typedef struct mrb_leapmotion_frame_t_ {
  Leap::Frame const *obj;
} mrb_leapmotion_frame_t;

typedef struct mrb_leapmotion_handlist_t_ {
  Leap::HandList const *obj;
} mrb_leapmotion_handlist_t;

typedef struct mrb_leapmotion_hand_t_ {
  Leap::Hand const *obj;
} mrb_leapmotion_hand_t;

typedef struct mrb_leapmotion_pointable_t_ {
  Leap::Pointable const *obj;
} mrb_leapmotion_pointable_t;

typedef struct mrb_leapmotion_fingerlist_t_ {
  Leap::FingerList const *obj;
} mrb_leapmotion_fingerlist_t;

typedef struct mrb_leapmotion_finger_t_ {
  Leap::Finger const *obj;
} mrb_leapmotion_finger_t;

/*
 * Generic type mapping.
 */
template <typename T> struct mrb_leapmotion_type_map_t {
};
template <> struct mrb_leapmotion_type_map_t<Leap::Vector> {
public:
  typedef mrb_leapmotion_vector_t mrb_type;
  static RClass *mrb_class;
  static mrb_data_type * const mrb_obj_data_type;
};
template <> struct mrb_leapmotion_type_map_t<Leap::Frame> {
public:
  typedef mrb_leapmotion_frame_t mrb_type;
  static RClass *mrb_class;
  static mrb_data_type * const mrb_obj_data_type;
};
template <> struct mrb_leapmotion_type_map_t<Leap::HandList> {
public:
  typedef mrb_leapmotion_handlist_t mrb_type;
  static RClass *mrb_class;
  static mrb_data_type * const mrb_obj_data_type;
};
template <> struct mrb_leapmotion_type_map_t<Leap::Hand> {
public:
  typedef mrb_leapmotion_hand_t mrb_type;
  static RClass *mrb_class;
  static mrb_data_type * const mrb_obj_data_type;
};
template <> struct mrb_leapmotion_type_map_t<Leap::Pointable> {
public:
  typedef mrb_leapmotion_pointable_t mrb_type;
  static RClass *mrb_class;
  static mrb_data_type * const mrb_obj_data_type;
};
template <> struct mrb_leapmotion_type_map_t<Leap::FingerList> {
public:
  typedef mrb_leapmotion_fingerlist_t mrb_type;
  static RClass *mrb_class;
  static mrb_data_type * const mrb_obj_data_type;
};
template <> struct mrb_leapmotion_type_map_t<Leap::Finger> {
public:
  typedef mrb_leapmotion_finger_t mrb_type;
  static RClass *mrb_class;
  static mrb_data_type * const mrb_obj_data_type;
};

RClass *mrb_leapmotion_type_map_t<Leap::Vector>::mrb_class = NULL;
mrb_data_type * const mrb_leapmotion_type_map_t<Leap::Vector>::mrb_obj_data_type = &mrb_leapmotion_vector_type;
RClass *mrb_leapmotion_type_map_t<Leap::Frame>::mrb_class = NULL;
mrb_data_type * const mrb_leapmotion_type_map_t<Leap::Frame>::mrb_obj_data_type = &mrb_leapmotion_frame_type;
RClass *mrb_leapmotion_type_map_t<Leap::HandList>::mrb_class = NULL;
mrb_data_type * const mrb_leapmotion_type_map_t<Leap::HandList>::mrb_obj_data_type = &mrb_leapmotion_handlist_type;
RClass *mrb_leapmotion_type_map_t<Leap::Hand>::mrb_class = NULL;
mrb_data_type * const mrb_leapmotion_type_map_t<Leap::Hand>::mrb_obj_data_type = &mrb_leapmotion_hand_type;
RClass *mrb_leapmotion_type_map_t<Leap::Pointable>::mrb_class = NULL;
mrb_data_type * const mrb_leapmotion_type_map_t<Leap::Pointable>::mrb_obj_data_type = &mrb_leapmotion_pointable_type;
RClass *mrb_leapmotion_type_map_t<Leap::FingerList>::mrb_class = NULL;
mrb_data_type * const mrb_leapmotion_type_map_t<Leap::FingerList>::mrb_obj_data_type = &mrb_leapmotion_fingerlist_type;
RClass *mrb_leapmotion_type_map_t<Leap::Finger>::mrb_class = NULL;
mrb_data_type * const mrb_leapmotion_type_map_t<Leap::Finger>::mrb_obj_data_type = &mrb_leapmotion_finger_type;

/*
 * Proxy listener class to delivery events.
 */
class mrb_leapmotion_listener_proxy : public Leap::Listener {
public:
  mrb_leapmotion_listener_proxy(mrb_state *mrb, mrb_value &self) : mrb_(mrb), listener_(self) {
  }
  virtual void onInit(Leap::Controller const &c) {
    mrb_value controller = mrb_iv_get(mrb_, listener_, mrb_intern(mrb_, "controller"));
    mrb_funcall(mrb_, listener_, "on_init", 1, controller);
  }
  virtual void onConnect(Leap::Controller const &c) {
    mrb_value controller = mrb_iv_get(mrb_, listener_, mrb_intern(mrb_, "controller"));
    mrb_funcall(mrb_, listener_, "on_connect", 1, controller);
  }
  virtual void onDisconnect(Leap::Controller const &c) {
    mrb_value controller = mrb_iv_get(mrb_, listener_, mrb_intern(mrb_, "controller"));
    mrb_funcall(mrb_, listener_, "on_disconnect", 1, controller);
  }
  virtual void onExit(Leap::Controller const &c) {
    mrb_value controller = mrb_iv_get(mrb_, listener_, mrb_intern(mrb_, "controller"));
    mrb_funcall(mrb_, listener_, "on_exit", 1, controller);
  }
  virtual void onFrame(Leap::Controller const &c) {
    mrb_value controller = mrb_iv_get(mrb_, listener_, mrb_intern(mrb_, "controller"));
    mrb_funcall(mrb_, listener_, "on_frame", 1, controller);
  }
  virtual void onFocusGained(Leap::Controller const &c) {
    mrb_value controller = mrb_iv_get(mrb_, listener_, mrb_intern(mrb_, "controller"));
    mrb_funcall(mrb_, listener_, "on_focus_gained", 1, controller);
  }
  virtual void onFocusLost(Leap::Controller const &c) {
    mrb_value controller = mrb_iv_get(mrb_, listener_, mrb_intern(mrb_, "controller"));
    mrb_funcall(mrb_, listener_, "on_focus_lost", 1, controller);
  }
private:
  mrb_state *mrb_;
  mrb_value listener_;
};

template <typename TLeap> static mrb_value
mrb_leapmotion_obj_make(mrb_state *mrb, TLeap const &obj)
{
  typename mrb_leapmotion_type_map_t<TLeap>::mrb_type *data =
    static_cast<typename mrb_leapmotion_type_map_t<TLeap>::mrb_type*>(mrb_malloc(mrb, sizeof(typename mrb_leapmotion_type_map_t<TLeap>::mrb_type)));
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot allocate frame object.");
  }
  try {
    data->obj = new TLeap(obj);
  } catch (std::exception &ex) {
    mrb_free(mrb, data);
    data = NULL;
  } catch (...) {
    mrb_free(mrb, data);
    data = NULL;
  }
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot create frame object.");
  }
  return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_leapmotion_type_map_t<TLeap>::mrb_class, mrb_leapmotion_type_map_t<TLeap>::mrb_obj_data_type, data));
}

extern "C" {

static void
mrb_leapmotion_controller_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_controller_t *data = static_cast<mrb_leapmotion_controller_t*>(ptr);
  if (data != nullptr) {
    delete data->controller;
    data->controller = nullptr;
  }
  mrb_free(mrb, ptr);
}

static mrb_leapmotion_controller_t*
mrb_leapmotion_controller_alloc(mrb_state *mrb)
{
  mrb_leapmotion_controller_t *data = static_cast<mrb_leapmotion_controller_t*>(mrb_malloc(mrb, sizeof(mrb_leapmotion_controller_t)));
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot allocate memory.");
  }
  try {
    data->controller = new Leap::Controller(); // support only default construction.
  } catch (std::exception &ex) {
    mrb_free(mrb, data);
    data = NULL;
  } catch (...) {
    mrb_free(mrb, data);
    data = NULL;
  }
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot construct an object.");
  }
  return data;
}

/*
 * mruby's data free functions.
 */

static void
mrb_leapmotion_listener_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_listener_t *data = static_cast<mrb_leapmotion_listener_t*>(ptr);
  if (data != nullptr) {
    delete data->listener;
    data->listener = nullptr;
  }
  mrb_free(mrb, ptr);
}

static mrb_leapmotion_listener_t*
mrb_leapmotion_listener_alloc(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_listener_t *data = static_cast<mrb_leapmotion_listener_t*>(mrb_malloc(mrb, sizeof(mrb_leapmotion_listener_t)));
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot allocate memory.");
  }
  try {
    data->listener = new mrb_leapmotion_listener_proxy(mrb, self); // generate proxy class.
  } catch (std::exception &ex) {
    mrb_free(mrb, data);
    data = NULL;
  }
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot construct an object.");
  }
  return data;
}

static void
mrb_leapmotion_vector_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_vector_t *data = static_cast<mrb_leapmotion_vector_t*>(ptr);
  if (data != nullptr) {
    delete data->obj;
    data->obj = nullptr;
  }
  mrb_free(mrb, ptr);
}

static void
mrb_leapmotion_frame_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_frame_t *data = static_cast<mrb_leapmotion_frame_t*>(ptr);
  if (data != nullptr) {
    delete data->obj;
    data->obj = nullptr;
  }
  mrb_free(mrb, ptr);
}

static void
mrb_leapmotion_handlist_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_handlist_t *data = static_cast<mrb_leapmotion_handlist_t*>(ptr);
  if (data != nullptr) {
    delete data->obj;
    data->obj = nullptr;
  }
  mrb_free(mrb, ptr);
}

static void
mrb_leapmotion_hand_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_hand_t *data = static_cast<mrb_leapmotion_hand_t*>(ptr);
  if (data != nullptr) {
    delete data->obj;
    data->obj = nullptr;
  }
  mrb_free(mrb, ptr);
}

static void
mrb_leapmotion_pointable_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_pointable_t *data = static_cast<mrb_leapmotion_pointable_t*>(ptr);
  if (data != nullptr) {
    delete data->obj;
    data->obj = nullptr;
  }
  mrb_free(mrb, ptr);
}

static void
mrb_leapmotion_fingerlist_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_fingerlist_t *data = static_cast<mrb_leapmotion_fingerlist_t*>(ptr);
  if (data != nullptr) {
    delete data->obj;
    data->obj = nullptr;
  }
  mrb_free(mrb, ptr);
}

static void
mrb_leapmotion_finger_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_finger_t *data = static_cast<mrb_leapmotion_finger_t*>(ptr);
  if (data != nullptr) {
    delete data->obj;
    data->obj = nullptr;
  }
  mrb_free(mrb, ptr);
}

/*
 * LeapMotion::Listener
 */

static mrb_value
mrb_leapmotion_controller_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_controller_t *data = static_cast<mrb_leapmotion_controller_t*>(DATA_PTR(self));

  if (data != NULL) {
    mrb_leapmotion_controller_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_leapmotion_controller_type;
  DATA_PTR(self) = mrb_leapmotion_controller_alloc(mrb);

  return self;
}

static mrb_value
mrb_leapmotion_controller_add_listener(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_controller_t *cdata = static_cast<mrb_leapmotion_controller_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_controller_type));
  if (cdata == NULL) {
    return mrb_nil_value();
  }

  mrb_value listener;
  mrb_get_args(mrb, "o", &listener);
  if (mrb_type(listener) != MRB_TT_DATA) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "incompatible type argument is supplied.");
  }

  mrb_leapmotion_listener_t *ldata = static_cast<mrb_leapmotion_listener_t*>(mrb_data_get_ptr(mrb, listener, &mrb_leapmotion_listener_type));
  if (ldata == NULL) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "incomptible data type argument is suppliced.");
  }

  mrb_iv_set(mrb, listener, mrb_intern(mrb, "controller"), self);

  return cdata->controller->addListener(*ldata->listener) ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_controller_remove_listener(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_controller_t *cdata = static_cast<mrb_leapmotion_controller_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_controller_type));
  if (cdata == NULL) {
    return mrb_nil_value();
  }

  mrb_value listener;
  mrb_get_args(mrb, "o", &listener);
  if (mrb_type(listener) != MRB_TT_DATA) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "incompatible type argument is supplied.");
  }

  mrb_leapmotion_listener_t *ldata = static_cast<mrb_leapmotion_listener_t*>(mrb_data_get_ptr(mrb, listener, &mrb_leapmotion_listener_type));
  if (ldata == NULL) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "incomptible data type argument is suppliced.");
  }

  if (cdata->controller->removeListener(*ldata->listener)) {
    mrb_iv_set(mrb, listener, mrb_intern(mrb, "controller"), mrb_nil_value());
    return mrb_true_value();
  }

  return mrb_false_value();
}

static mrb_value
mrb_leapmotion_controller_frame(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_controller_t *data = static_cast<mrb_leapmotion_controller_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_controller_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->controller->frame());
}

static mrb_value
mrb_leapmotion_controller_config(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_controller_devices(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_controller_is_connected(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_controller_t *data = static_cast<mrb_leapmotion_controller_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_controller_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->controller->isConnected() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_controller_has_focus(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_controller_t *data = static_cast<mrb_leapmotion_controller_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_controller_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->controller->hasFocus() ? mrb_true_value() : mrb_false_value();
}

/*
 * LeapMotion::Listener class methods
 */

static mrb_value
mrb_leapmotion_listener_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_listener_t *data = static_cast<mrb_leapmotion_listener_t*>(DATA_PTR(self));

  if (data != NULL) {
    mrb_leapmotion_listener_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_leapmotion_listener_type;
  DATA_PTR(self) = mrb_leapmotion_listener_alloc(mrb, self);

  return self;
}

static mrb_value
mrb_leapmotion_listener_on_init(mrb_state *mrb, mrb_value self)
{
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_listener_on_connect(mrb_state *mrb, mrb_value self)
{
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_listener_on_disconnect(mrb_state *mrb, mrb_value self)
{
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_listener_on_exit(mrb_state *mrb, mrb_value self)
{
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_listener_on_frame(mrb_state *mrb, mrb_value self)
{
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_listener_on_focus_gained(mrb_state *mrb, mrb_value self)
{
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_listener_on_focus_lost(mrb_state *mrb, mrb_value self)
{
  return mrb_nil_value();
}

/*
 * LeapMotion::Vector
 */

static mrb_value
mrb_leapmotion_vector_magnitude(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_vector_magnitude_squared(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_vector_distance_to(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_vector_angle_to(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_vector_pitch(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data = static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->pitch());
}

static mrb_value
mrb_leapmotion_vector_yaw(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data = static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->yaw());
}

static mrb_value
mrb_leapmotion_vector_roll(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data = static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->roll());
}

static mrb_value
mrb_leapmotion_vector_dot(mrb_state *mrb, mrb_value self)
{
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_vector_is_valid(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data = static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->isValid() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_vector_get_at(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data = static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  return mrb_float_value(mrb, data->obj->operator[](index));
}

static mrb_value
mrb_leapmotion_vector_to_s(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data = static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  std::string const str(std::move(data->obj->toString()));
  return mrb_str_new(mrb, str.c_str(), str.length());
}

static mrb_value
mrb_leapmotion_vector_get_x(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data = static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->x);
}

static mrb_value
mrb_leapmotion_vector_get_y(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data = static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->y);
}

static mrb_value
mrb_leapmotion_vector_get_z(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data = static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->z);
}

/*
 * LeapMotion::Frame
 */

static mrb_value
mrb_leapmotion_frame_id(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data = static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, (mrb_float)data->obj->id());
}

static mrb_value
mrb_leapmotion_frame_timestamp(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data = static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, (mrb_float)data->obj->timestamp());
}

static mrb_value
mrb_leapmotion_frame_hands(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data = static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->hands());
}

static mrb_value
mrb_leapmotion_frame_hand(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data = static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  return mrb_leapmotion_obj_make(mrb, data->obj->hand(index));
}

static mrb_value
mrb_leapmotion_frame_is_valid(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data = static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->isValid() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_frame_to_s(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data = static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  std::string const str(std::move(data->obj->toString()));
  return mrb_str_new(mrb, str.c_str(), str.length());
}

/*
 * LeapMotion::HandList
 */

static mrb_value
mrb_leapmotion_handlist_count(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_handlist_t *data = static_cast<mrb_leapmotion_handlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_handlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_fixnum_value(data->obj->count());
}

static mrb_value
mrb_leapmotion_handlist_is_empty(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_handlist_t *data = static_cast<mrb_leapmotion_handlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_handlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->empty() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_handlist_get_at(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_handlist_t *data = static_cast<mrb_leapmotion_handlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_handlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  return mrb_leapmotion_obj_make(mrb, data->obj->operator[](index));
}

static mrb_value
mrb_leapmotion_handlist_leftmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_handlist_t *data = static_cast<mrb_leapmotion_handlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_handlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->leftmost());
}

static mrb_value
mrb_leapmotion_handlist_rightmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_handlist_t *data = static_cast<mrb_leapmotion_handlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_handlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->rightmost());
}

static mrb_value
mrb_leapmotion_handlist_frontmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_handlist_t *data = static_cast<mrb_leapmotion_handlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_handlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->frontmost());
}

/*
 * LeapMotion::Hand
 */

static mrb_value
mrb_leapmotion_hand_id(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data = static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->id());
}

static mrb_value
mrb_leapmotion_hand_fingers(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data = static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->fingers());
}

static mrb_value
mrb_leapmotion_hand_finger(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data = static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  return mrb_leapmotion_obj_make(mrb, data->obj->finger(index));
}

static mrb_value
mrb_leapmotion_hand_sphere_radius(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data = static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->sphereRadius());
}

static mrb_value
mrb_leapmotion_hand_is_valid(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data = static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->isValid() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_hand_to_s(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data = static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  std::string const str(std::move(data->obj->toString()));
  return mrb_str_new(mrb, str.c_str(), str.length());
}

/*
 * LeapMotion::FingerList
 */

static mrb_value
mrb_leapmotion_fingerlist_count(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_fingerlist_t *data = static_cast<mrb_leapmotion_fingerlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_fingerlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_fixnum_value(data->obj->count());
}

static mrb_value
mrb_leapmotion_fingerlist_is_empty(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_fingerlist_t *data = static_cast<mrb_leapmotion_fingerlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_fingerlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->empty() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_fingerlist_get_at(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_fingerlist_t *data = static_cast<mrb_leapmotion_fingerlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_fingerlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  return mrb_leapmotion_obj_make(mrb, data->obj->operator[](index));
}

static mrb_value
mrb_leapmotion_fingerlist_leftmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_fingerlist_t *data = static_cast<mrb_leapmotion_fingerlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_fingerlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->leftmost());
}

static mrb_value
mrb_leapmotion_fingerlist_rightmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_fingerlist_t *data = static_cast<mrb_leapmotion_fingerlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_fingerlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->rightmost());
}

static mrb_value
mrb_leapmotion_fingerlist_frontmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_fingerlist_t *data = static_cast<mrb_leapmotion_fingerlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_fingerlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->frontmost());
}

/*
 * LeapMotion::Pointable
 */

static mrb_value
mrb_leapmotion_pointable_id(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = static_cast<mrb_leapmotion_pointable_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointable_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->id());
}

static mrb_value
mrb_leapmotion_pointable_tip_position(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = static_cast<mrb_leapmotion_pointable_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointable_type));
  if (data == NULL) {
    mrb_leapmotion_finger_t *fdata = static_cast<mrb_leapmotion_finger_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_finger_type));
    if (fdata != NULL) {
      return mrb_leapmotion_obj_make(mrb, fdata->obj->tipPosition());
    }
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->tipPosition());
}

static mrb_value
mrb_leapmotion_pointable_width(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = static_cast<mrb_leapmotion_pointable_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointable_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->width());
}

static mrb_value
mrb_leapmotion_pointable_length(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = static_cast<mrb_leapmotion_pointable_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointable_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->length());
}

static mrb_value
mrb_leapmotion_pointable_is_finger(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = static_cast<mrb_leapmotion_pointable_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointable_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->isFinger() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_pointable_is_tool(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = static_cast<mrb_leapmotion_pointable_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointable_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->isTool() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_pointable_is_valid(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = static_cast<mrb_leapmotion_pointable_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointable_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->isValid() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_pointable_touch_distance(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = static_cast<mrb_leapmotion_pointable_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointable_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->touchDistance());
}

static mrb_value
mrb_leapmotion_pointable_to_s(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = static_cast<mrb_leapmotion_pointable_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointable_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  std::string const str(std::move(data->obj->toString()));
  return mrb_str_new(mrb, str.c_str(), str.length());
}

/*
 * LeapMotion::Finger
 */

static mrb_value
mrb_leapmotion_finger_to_s(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_finger_t *data = static_cast<mrb_leapmotion_finger_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_finger_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  std::string const str(std::move(data->obj->toString()));
  return mrb_str_new(mrb, str.c_str(), str.length());
}

/*
 * Kernel::sleep for Debug
 */

static mrb_value
mrb_kernel_sleep(mrb_state *mrb, mrb_value self)
{
  mrb_int period;
  mrb_get_args(mrb, "i", &period);
#if defined(_MSC_VER)
  ::Sleep(period);
#else
  if (0 != sleep(period)) {
    return mrb_false_value();
  }
#endif
  return mrb_true_value();
}

/*
 * exported entry points (init & final) for mrbgem.
 */

void
mrb_mruby_leapmotion_gem_init(mrb_state *mrb)
{
  top_module       = mrb_define_module(mrb, "LeapMotion");
  class_controller = mrb_define_class_under(mrb, top_module, "Controller", mrb->object_class);
  class_listener   = mrb_define_class_under(mrb, top_module, "Listener",   mrb->object_class);
  class_config     = mrb_define_class_under(mrb, top_module, "Config",     mrb->object_class);
  class_devicelist = mrb_define_class_under(mrb, top_module, "DeviceList", mrb->object_class);
  RClass *class_vector     = mrb_define_class_under(mrb, top_module, "Vector",     mrb->object_class);
  RClass *class_frame      = mrb_define_class_under(mrb, top_module, "Frame",      mrb->object_class);
  RClass *class_handlist   = mrb_define_class_under(mrb, top_module, "HandList",   mrb->object_class);
  RClass *class_hand       = mrb_define_class_under(mrb, top_module, "Hand",       mrb->object_class);
  RClass *class_pointable  = mrb_define_class_under(mrb, top_module, "Pointable",  mrb->object_class);
  RClass *class_fingerlist = mrb_define_class_under(mrb, top_module, "FingerList", mrb->object_class);
  RClass *class_finger     = mrb_define_class_under(mrb, top_module, "Finger",     class_pointable);

  MRB_SET_INSTANCE_TT(class_controller, MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_listener,   MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_config,     MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_devicelist, MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_vector,     MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_frame,      MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_handlist,   MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_hand,       MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_pointable,  MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_fingerlist, MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_finger,     MRB_TT_DATA);

  mrb_define_method(mrb, class_controller, "initialize",      mrb_leapmotion_controller_initialize,      ARGS_NONE());
  mrb_define_method(mrb, class_controller, "add_listener",    mrb_leapmotion_controller_add_listener,    ARGS_REQ(1));
  mrb_define_method(mrb, class_controller, "remove_listener", mrb_leapmotion_controller_remove_listener, ARGS_REQ(1));
  mrb_define_method(mrb, class_controller, "frame",           mrb_leapmotion_controller_frame,           ARGS_REQ(1));
  mrb_define_method(mrb, class_controller, "config",          mrb_leapmotion_controller_config,          ARGS_NONE());
  mrb_define_method(mrb, class_controller, "devices",         mrb_leapmotion_controller_devices,         ARGS_NONE());
  mrb_define_method(mrb, class_controller, "connected?",      mrb_leapmotion_controller_is_connected,    ARGS_NONE());
  mrb_define_method(mrb, class_controller, "has_focus?",      mrb_leapmotion_controller_has_focus,       ARGS_NONE());

  mrb_define_method(mrb, class_listener, "initialize",      mrb_leapmotion_listener_initialize,      ARGS_NONE());
  mrb_define_method(mrb, class_listener, "on_init",         mrb_leapmotion_listener_on_init,         ARGS_REQ(1));
  mrb_define_method(mrb, class_listener, "on_connect",      mrb_leapmotion_listener_on_connect,      ARGS_REQ(1));
  mrb_define_method(mrb, class_listener, "on_disconnect",   mrb_leapmotion_listener_on_disconnect,   ARGS_REQ(1));
  mrb_define_method(mrb, class_listener, "on_exit",         mrb_leapmotion_listener_on_exit,         ARGS_REQ(1));
  mrb_define_method(mrb, class_listener, "on_frame",        mrb_leapmotion_listener_on_frame,        ARGS_REQ(1));
  mrb_define_method(mrb, class_listener, "on_focus_gained", mrb_leapmotion_listener_on_focus_gained, ARGS_REQ(1));
  mrb_define_method(mrb, class_listener, "on_focus_lost",   mrb_leapmotion_listener_on_focus_lost,   ARGS_REQ(1));

  mrb_define_method(mrb, class_vector, "magnitude",         mrb_leapmotion_vector_magnitude,         ARGS_NONE());
  mrb_define_method(mrb, class_vector, "magnitude_squared", mrb_leapmotion_vector_magnitude_squared, ARGS_NONE());
  mrb_define_method(mrb, class_vector, "distance_to",       mrb_leapmotion_vector_distance_to,       ARGS_REQ(1));
  mrb_define_method(mrb, class_vector, "angle_to",          mrb_leapmotion_vector_angle_to,          ARGS_REQ(1));
  mrb_define_method(mrb, class_vector, "pitch",             mrb_leapmotion_vector_pitch,             ARGS_NONE());
  mrb_define_method(mrb, class_vector, "yaw",               mrb_leapmotion_vector_yaw,               ARGS_NONE());
  mrb_define_method(mrb, class_vector, "roll",              mrb_leapmotion_vector_roll,              ARGS_NONE());
  mrb_define_method(mrb, class_vector, "dot",               mrb_leapmotion_vector_dot,               ARGS_REQ(1));
  mrb_define_method(mrb, class_vector, "valid?",            mrb_leapmotion_vector_is_valid,          ARGS_NONE());
  mrb_define_method(mrb, class_vector, "[]",                mrb_leapmotion_vector_get_at,            ARGS_REQ(1));
  mrb_define_method(mrb, class_vector, "to_s",              mrb_leapmotion_vector_to_s,              ARGS_NONE());
  mrb_define_method(mrb, class_vector, "x",                 mrb_leapmotion_vector_get_x,             ARGS_NONE());
  mrb_define_method(mrb, class_vector, "y",                 mrb_leapmotion_vector_get_y,             ARGS_NONE());
  mrb_define_method(mrb, class_vector, "z",                 mrb_leapmotion_vector_get_z,             ARGS_NONE());

  mrb_define_method(mrb, class_frame, "id",        mrb_leapmotion_frame_id,        ARGS_NONE());
  mrb_define_method(mrb, class_frame, "timestamp", mrb_leapmotion_frame_timestamp, ARGS_NONE());
  mrb_define_method(mrb, class_frame, "hands",     mrb_leapmotion_frame_hands,     ARGS_NONE());
  mrb_define_method(mrb, class_frame, "hand",      mrb_leapmotion_frame_hand,      ARGS_REQ(1));
  mrb_define_method(mrb, class_frame, "valid?",    mrb_leapmotion_frame_is_valid,  ARGS_NONE());
  mrb_define_method(mrb, class_frame, "to_s",      mrb_leapmotion_frame_to_s,      ARGS_NONE());

  mrb_define_method(mrb, class_handlist, "count" ,    mrb_leapmotion_handlist_count,     ARGS_NONE());
  mrb_define_method(mrb, class_handlist, "empty?",    mrb_leapmotion_handlist_is_empty,  ARGS_NONE());
  mrb_define_method(mrb, class_handlist, "[]",        mrb_leapmotion_handlist_get_at,    ARGS_REQ(1));
  mrb_define_method(mrb, class_handlist, "leftmost",  mrb_leapmotion_handlist_leftmost,  ARGS_NONE());
  mrb_define_method(mrb, class_handlist, "rightmost", mrb_leapmotion_handlist_rightmost, ARGS_NONE());
  mrb_define_method(mrb, class_handlist, "frontmost", mrb_leapmotion_handlist_frontmost, ARGS_NONE());

  mrb_define_method(mrb, class_hand, "id",            mrb_leapmotion_hand_id,            ARGS_NONE());
  mrb_define_method(mrb, class_hand, "fingers",       mrb_leapmotion_hand_fingers,       ARGS_NONE());
  mrb_define_method(mrb, class_hand, "finger",        mrb_leapmotion_hand_finger,        ARGS_REQ(1));
  mrb_define_method(mrb, class_hand, "sphere_radius", mrb_leapmotion_hand_sphere_radius, ARGS_NONE());
  mrb_define_method(mrb, class_hand, "valid?",        mrb_leapmotion_hand_is_valid,      ARGS_NONE());
  mrb_define_method(mrb, class_hand, "to_s",          mrb_leapmotion_hand_to_s,          ARGS_NONE());

  mrb_define_method(mrb, class_pointable, "id",             mrb_leapmotion_pointable_id,             ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "tip_position",   mrb_leapmotion_pointable_tip_position,   ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "width",          mrb_leapmotion_pointable_width,          ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "length",         mrb_leapmotion_pointable_length,         ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "finger?",        mrb_leapmotion_pointable_is_finger,      ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "tool?",          mrb_leapmotion_pointable_is_tool,        ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "valid?",         mrb_leapmotion_pointable_is_valid,       ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "touch_distance", mrb_leapmotion_pointable_touch_distance, ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "to_s",           mrb_leapmotion_pointable_to_s,           ARGS_NONE());

  mrb_define_method(mrb, class_fingerlist, "count",     mrb_leapmotion_fingerlist_count,     ARGS_NONE());
  mrb_define_method(mrb, class_fingerlist, "empty?",    mrb_leapmotion_fingerlist_is_empty,  ARGS_NONE());
  mrb_define_method(mrb, class_fingerlist, "[]",        mrb_leapmotion_fingerlist_get_at,    ARGS_REQ(1));
  mrb_define_method(mrb, class_fingerlist, "leftmost",  mrb_leapmotion_fingerlist_leftmost,  ARGS_NONE());
  mrb_define_method(mrb, class_fingerlist, "rightmost", mrb_leapmotion_fingerlist_rightmost, ARGS_NONE());
  mrb_define_method(mrb, class_fingerlist, "frontmost", mrb_leapmotion_fingerlist_frontmost, ARGS_NONE());

  mrb_define_method(mrb, class_finger, "to_s", mrb_leapmotion_finger_to_s, ARGS_NONE());

  /* for debug */
  mrb_define_method(mrb, mrb->kernel_module, "sleep", mrb_kernel_sleep, ARGS_REQ(1));

  mrb_leapmotion_type_map_t<Leap::Vector>::mrb_class     = class_vector;
  mrb_leapmotion_type_map_t<Leap::Frame>::mrb_class      = class_frame;
  mrb_leapmotion_type_map_t<Leap::HandList>::mrb_class   = class_handlist;
  mrb_leapmotion_type_map_t<Leap::Hand>::mrb_class       = class_hand;
  mrb_leapmotion_type_map_t<Leap::Pointable>::mrb_class  = class_pointable;
  mrb_leapmotion_type_map_t<Leap::FingerList>::mrb_class = class_fingerlist;
  mrb_leapmotion_type_map_t<Leap::Finger>::mrb_class     = class_finger;
}

void
mrb_mruby_leapmotion_gem_final(mrb_state *mrb)
{
}

}

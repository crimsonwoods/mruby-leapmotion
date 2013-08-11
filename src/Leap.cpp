#include "mruby.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/variable.h"
#include "mruby/string.h"
#include "Leap.h"
#include <utility>
#include <memory>
#if defined(_MSC_VER)
#include <windows.h>
#else
#include <unistd.h>
#endif

template <typename T> struct mrb_leapmotion_type_map_t {
};

#define def_mruby_leap_class(type_name, name_str, leap_type) \
extern "C" { static void mrb_leapmotion_##type_name##_free(mrb_state *mrb, void *ptr); }\
static struct mrb_data_type mrb_leapmotion_##type_name##_type = { name_str, mrb_leapmotion_##type_name##_free }; \
typedef struct mrb_leapmotion_##type_name##_t_ { \
  leap_type *obj; \
} mrb_leapmotion_##type_name##_t

#define def_mruby_leap_derived_class(outer_type_name, type_name, name_str, leap_type) \
extern "C" { static void mrb_leapmotion_##type_name##_free(mrb_state *mrb, void *ptr); }\
static struct mrb_data_type mrb_leapmotion_##type_name##_type = { name_str, mrb_leapmotion_##type_name##_free }; \
typedef struct mrb_leapmotion_##type_name##_t_ : public mrb_leapmotion_##outer_type_name##_t { \
} mrb_leapmotion_##type_name##_t

#define def_type_map(leap_type, mruby_type, data_type_obj) \
template <> struct mrb_leapmotion_type_map_t<leap_type> { \
public: \
  typedef mruby_type mrb_type; \
  static RClass *mrb_class; \
  static mrb_data_type * const mrb_obj_data_type; \
}; \
RClass *mrb_leapmotion_type_map_t<leap_type>::mrb_class = NULL; \
mrb_data_type * const mrb_leapmotion_type_map_t<leap_type>::mrb_obj_data_type = &data_type_obj

def_mruby_leap_class(controller,       "Controller",       Leap::Controller);
def_mruby_leap_class(config,           "Config",           Leap::Config);
def_mruby_leap_class(listener,         "Listener",         Leap::Listener);
def_mruby_leap_class(vector,           "Vector",           Leap::Vector);
def_mruby_leap_class(frame,            "Frame",            Leap::Frame);
def_mruby_leap_class(fingerlist,       "FingerList",       Leap::FingerList);
def_mruby_leap_class(handlist,         "HandList",         Leap::HandList);
def_mruby_leap_class(hand,             "Hand",             Leap::Hand);
def_mruby_leap_class(pointablelist,    "PointableList",    Leap::PointableList);
def_mruby_leap_class(pointable,        "Pointable",        Leap::Pointable);
def_mruby_leap_class(toollist,         "ToolList",         Leap::ToolList);
def_mruby_leap_derived_class(pointable, tool,   "Tool",    Leap::Tool);
def_mruby_leap_derived_class(pointable, finger, "Finger",  Leap::Finger);
def_mruby_leap_class(gesturelist,      "GestureList",      Leap::GestureList);
def_mruby_leap_class(gesture,          "Gesture",          Leap::Gesture);
def_mruby_leap_derived_class(gesture, swipegesture,     "SwipeGesture",     Leap::Gesture);
def_mruby_leap_derived_class(gesture, circlegesture,    "CircleGesture",    Leap::Gesture);
def_mruby_leap_derived_class(gesture, screentapgesture, "ScreenTapGesture", Leap::Gesture);
def_mruby_leap_derived_class(gesture, keytapgesture,    "KeyTapGesture",    Leap::Gesture);

def_type_map(Leap::Controller,       mrb_leapmotion_controller_t,       mrb_leapmotion_controller_type);
def_type_map(Leap::Config,           mrb_leapmotion_config_t,           mrb_leapmotion_config_type);
def_type_map(Leap::Listener,         mrb_leapmotion_listener_t,         mrb_leapmotion_listener_type);
def_type_map(Leap::Vector,           mrb_leapmotion_vector_t,           mrb_leapmotion_vector_type);
def_type_map(Leap::Frame,            mrb_leapmotion_frame_t,            mrb_leapmotion_frame_type);
def_type_map(Leap::HandList,         mrb_leapmotion_handlist_t,         mrb_leapmotion_handlist_type);
def_type_map(Leap::Hand,             mrb_leapmotion_hand_t,             mrb_leapmotion_hand_type);
def_type_map(Leap::PointableList,    mrb_leapmotion_pointablelist_t,    mrb_leapmotion_pointablelist_type);
def_type_map(Leap::Pointable,        mrb_leapmotion_pointable_t,        mrb_leapmotion_pointable_type);
def_type_map(Leap::ToolList,         mrb_leapmotion_toollist_t,         mrb_leapmotion_toollist_type);
def_type_map(Leap::Tool,             mrb_leapmotion_tool_t,             mrb_leapmotion_tool_type);
def_type_map(Leap::FingerList,       mrb_leapmotion_fingerlist_t,       mrb_leapmotion_fingerlist_type);
def_type_map(Leap::Finger,           mrb_leapmotion_finger_t,           mrb_leapmotion_finger_type);
def_type_map(Leap::GestureList,      mrb_leapmotion_gesturelist_t,      mrb_leapmotion_gesturelist_type);
def_type_map(Leap::Gesture,          mrb_leapmotion_gesture_t,          mrb_leapmotion_gesture_type);
def_type_map(Leap::SwipeGesture,     mrb_leapmotion_swipegesture_t,     mrb_leapmotion_swipegesture_type);
def_type_map(Leap::CircleGesture,    mrb_leapmotion_circlegesture_t,    mrb_leapmotion_circlegesture_type);
def_type_map(Leap::ScreenTapGesture, mrb_leapmotion_screentapgesture_t, mrb_leapmotion_screentapgesture_type);
def_type_map(Leap::KeyTapGesture,    mrb_leapmotion_keytapgesture_t,    mrb_leapmotion_keytapgesture_type);

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

/*
 * Helper functions for mruby object creation.
 */
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

template <typename TMrb> static void
mrb_leapmotion_object_free(mrb_state *mrb, TMrb *ptr)
{
  if (ptr != nullptr) {
    delete ptr->obj;
    ptr->obj = nullptr;
  }
  mrb_free(mrb, static_cast<void*>(ptr));
}

template <typename TLeap> static typename mrb_leapmotion_type_map_t<TLeap>::mrb_type*
mrb_leapmotion_gesture_alloc(mrb_state *mrb, mrb_leapmotion_gesture_t *gesture)
{
  typename mrb_leapmotion_type_map_t<TLeap>::mrb_type *data =
    static_cast<typename mrb_leapmotion_type_map_t<TLeap>::mrb_type*>(mrb_malloc(mrb, sizeof(typename mrb_leapmotion_type_map_t<TLeap>::mrb_type)));
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot allocate memory.");
  }
  try {
    data->obj = new TLeap(*gesture->obj);
  } catch (std::exception &ex) {
    mrb_free(mrb, data);
    data = NULL;
  }
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot construct an object.");
  }
  return data;
}

static mrb_leapmotion_vector_t*
mrb_leapmotion_vector_alloc(mrb_state *mrb)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_malloc(mrb, sizeof(mrb_leapmotion_vector_t)));
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot allocate memory.");
  }
  try {
    data->obj = new Leap::Vector();
  } catch (std::exception &ex) {
    mrb_free(mrb, data);
    data = NULL;
  }
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot construct an object.");
  }
  return data;
}

static mrb_leapmotion_vector_t*
mrb_leapmotion_vector_alloc(mrb_state *mrb, float x, float y, float z)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_malloc(mrb, sizeof(mrb_leapmotion_vector_t)));
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot allocate memory.");
  }
  try {
    data->obj = new Leap::Vector(x, y, z);
  } catch (std::exception &ex) {
    mrb_free(mrb, data);
    data = NULL;
  }
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot construct an object.");
  }
  return data;
}

static mrb_leapmotion_vector_t*
mrb_leapmotion_vector_alloc(mrb_state *mrb, mrb_leapmotion_vector_t *vector)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_malloc(mrb, sizeof(mrb_leapmotion_vector_t)));
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot allocate memory.");
  }
  try {
    data->obj = new Leap::Vector(*vector->obj);
  } catch (std::exception &ex) {
    mrb_free(mrb, data);
    data = NULL;
  }
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot construct an object.");
  }
  return data;
}

/*
 * Functions that are required C linkage for mruby.
 */
extern "C" {

static mrb_leapmotion_controller_t*
mrb_leapmotion_controller_alloc(mrb_state *mrb)
{
  mrb_leapmotion_controller_t *data =
    static_cast<mrb_leapmotion_controller_t*>(mrb_malloc(mrb, sizeof(mrb_leapmotion_controller_t)));
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot allocate memory.");
  }
  try {
    data->obj = new Leap::Controller(); // support only default construction.
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

static mrb_leapmotion_listener_t*
mrb_leapmotion_listener_alloc(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_listener_t *data =
    static_cast<mrb_leapmotion_listener_t*>(mrb_malloc(mrb, sizeof(mrb_leapmotion_listener_t)));
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "cannot allocate memory.");
  }
  try {
    data->obj = new mrb_leapmotion_listener_proxy(mrb, self); // generate proxy class.
  } catch (std::exception &ex) {
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
mrb_leapmotion_controller_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_controller_t*>(ptr));
}

static void
mrb_leapmotion_config_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_config_t*>(ptr));
}

static void
mrb_leapmotion_listener_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_listener_t*>(ptr));
}

static void
mrb_leapmotion_vector_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_vector_t*>(ptr));
}

static void
mrb_leapmotion_frame_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_frame_t*>(ptr));
}

static void
mrb_leapmotion_handlist_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_handlist_t*>(ptr));
}

static void
mrb_leapmotion_hand_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_hand_t*>(ptr));
}

static void
mrb_leapmotion_pointablelist_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_pointablelist_t*>(ptr));
}

static void
mrb_leapmotion_pointable_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_pointable_t*>(ptr));
}

static void
mrb_leapmotion_toollist_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_toollist_t*>(ptr));
}

static void
mrb_leapmotion_tool_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_tool_t*>(ptr));
}

static void
mrb_leapmotion_fingerlist_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_fingerlist_t*>(ptr));
}

static void
mrb_leapmotion_finger_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_finger_t*>(ptr));
}

static void
mrb_leapmotion_gesturelist_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_gesturelist_t*>(ptr));
}

static void
mrb_leapmotion_gesture_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_gesture_t*>(ptr));
}

static void
mrb_leapmotion_swipegesture_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_swipegesture_t*>(ptr));
}

static void
mrb_leapmotion_circlegesture_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_circlegesture_t*>(ptr));
}

static void
mrb_leapmotion_screentapgesture_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_screentapgesture_t*>(ptr));
}

static void
mrb_leapmotion_keytapgesture_free(mrb_state *mrb, void *ptr)
{
  mrb_leapmotion_object_free(mrb, static_cast<mrb_leapmotion_keytapgesture_t*>(ptr));
}

/*
 * LeapMotion::Controller
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
mrb_leapmotion_controller_policy_flags(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_controller_t *data =
    static_cast<mrb_leapmotion_controller_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_controller_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_fixnum_value(data->obj->policyFlags());
}

static mrb_value
mrb_leapmotion_controller_set_policy_flags(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_controller_t *data =
    static_cast<mrb_leapmotion_controller_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_controller_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_int flags = 0;
  mrb_get_args(mrb, "i", &flags);

  data->obj->setPolicyFlags(static_cast<Leap::Controller::PolicyFlag>(flags));

  return mrb_nil_value();
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

  return cdata->obj->addListener(*ldata->obj) ? mrb_true_value() : mrb_false_value();
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

  if (cdata->obj->removeListener(*ldata->obj)) {
    mrb_iv_set(mrb, listener, mrb_intern(mrb, "controller"), mrb_nil_value());
    return mrb_true_value();
  }

  return mrb_false_value();
}

static mrb_value
mrb_leapmotion_controller_frame(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_controller_t *data =
    static_cast<mrb_leapmotion_controller_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_controller_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->frame());
}

static mrb_value
mrb_leapmotion_controller_config(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_controller_t *data =
    static_cast<mrb_leapmotion_controller_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_controller_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->config());
}

static mrb_value
mrb_leapmotion_controller_devices(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_controller_located_screens(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_controller_calibrated_screens(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_controller_enable_gesture(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_controller_t *data =
    static_cast<mrb_leapmotion_controller_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_controller_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_int type;
  mrb_bool enable = true;
  mrb_get_args(mrb, "i|b", &type, &enable);
  data->obj->enableGesture(static_cast<Leap::Gesture::Type>(type), enable);
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_controller_is_gesture_enabled(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_controller_t *data =
    static_cast<mrb_leapmotion_controller_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_controller_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_int type;
  mrb_get_args(mrb, "i", &type);
  return data->obj->isGestureEnabled(static_cast<Leap::Gesture::Type>(type)) ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_controller_is_connected(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_controller_t *data =
    static_cast<mrb_leapmotion_controller_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_controller_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->isConnected() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_controller_has_focus(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_controller_t *data =
    static_cast<mrb_leapmotion_controller_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_controller_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->hasFocus() ? mrb_true_value() : mrb_false_value();
}

/*
 * LeapMotion::Config
 */

static mrb_value
mrb_leapmotion_config_get_type(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_config_t *data =
    static_cast<mrb_leapmotion_config_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_config_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value arg;
  mrb_get_args(mrb, "S", &arg);
  return mrb_fixnum_value(data->obj->type(RSTRING_PTR(arg)));
}

static mrb_value
mrb_leapmotion_config_get_bool(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_config_t *data =
    static_cast<mrb_leapmotion_config_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_config_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value arg;
  mrb_get_args(mrb, "S", &arg);
  return data->obj->getBool(RSTRING_PTR(arg)) ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_config_set_bool(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_config_t *data =
    static_cast<mrb_leapmotion_config_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_config_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value key;
  mrb_bool value;
  mrb_get_args(mrb, "Sb", &key, &value);
  data->obj->setBool(RSTRING_PTR(key), value ? true : false);
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_config_get_int(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_config_t *data =
    static_cast<mrb_leapmotion_config_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_config_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value arg;
  mrb_get_args(mrb, "S", &arg);
  return mrb_fixnum_value(data->obj->getInt32(RSTRING_PTR(arg)));
}

static mrb_value
mrb_leapmotion_config_set_int(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_config_t *data =
    static_cast<mrb_leapmotion_config_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_config_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value key;
  mrb_int value;
  mrb_get_args(mrb, "Si", &key, &value);
  data->obj->setInt32(RSTRING_PTR(key), static_cast<int32_t>(value));
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_config_get_float(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_config_t *data =
    static_cast<mrb_leapmotion_config_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_config_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value arg;
  mrb_get_args(mrb, "S", &arg);
  return mrb_float_value(mrb, data->obj->getFloat(RSTRING_PTR(arg)));
}

static mrb_value
mrb_leapmotion_config_set_float(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_config_t *data =
    static_cast<mrb_leapmotion_config_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_config_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value key;
  mrb_float value;
  mrb_get_args(mrb, "Sf", &key, &value);
  data->obj->setFloat(RSTRING_PTR(key), static_cast<float>(value));
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_config_get_string(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_config_t *data =
    static_cast<mrb_leapmotion_config_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_config_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value arg;
  mrb_get_args(mrb, "S", &arg);
  std::string const ret(data->obj->getString(RSTRING_PTR(arg)));
  return mrb_str_new(mrb, ret.c_str(), ret.length());
}

static mrb_value
mrb_leapmotion_config_set_string(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_config_t *data =
    static_cast<mrb_leapmotion_config_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_config_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value key;
  mrb_value value;
  mrb_get_args(mrb, "SS", &key, &value);
  data->obj->setString(RSTRING_PTR(key), RSTRING_PTR(value));
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_config_save(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_config_t *data =
    static_cast<mrb_leapmotion_config_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_config_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->save() ? mrb_true_value() : mrb_false_value();
}

/*
 * LeapMotion::Listener
 */

static mrb_value
mrb_leapmotion_listener_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_listener_t *data =
    static_cast<mrb_leapmotion_listener_t*>(DATA_PTR(self));
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
mrb_leapmotion_vector_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int argc;
  mrb_value *argv;

  mrb_get_args(mrb, "*", &argv, &argc);
  if (argc != 0 && argc != 1 && argc != 3) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "invalid number of arguments.");
  }

  mrb_leapmotion_vector_t *data = static_cast<mrb_leapmotion_vector_t*>(DATA_PTR(self));
  if (data != NULL) {
    mrb_leapmotion_vector_free(mrb, data);
  }

  switch (argc) {
  case 0:
    data = mrb_leapmotion_vector_alloc(mrb);
    break;
  case 1:
    mrb_data_check_type(mrb, argv[0], &mrb_leapmotion_vector_type);
    data = mrb_leapmotion_vector_alloc(mrb, static_cast<mrb_leapmotion_vector_t*>(DATA_PTR(argv[0])));
    break;
  case 3:
    data = mrb_leapmotion_vector_alloc(mrb, mrb_float(argv[0]), mrb_float(argv[1]), mrb_float(argv[2]));
    break;
  }

  DATA_TYPE(self) = &mrb_leapmotion_vector_type;
  DATA_PTR(self) = data;

  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_vector_magnitude(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  } 
  return mrb_float_value(mrb, data->obj->magnitude());
}

static mrb_value
mrb_leapmotion_vector_magnitude_squared(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  } 
  return mrb_float_value(mrb, data->obj->magnitudeSquared());
}

static mrb_value
mrb_leapmotion_vector_distance_to(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  mrb_data_check_type(mrb, arg, &mrb_leapmotion_vector_type);

  mrb_leapmotion_vector_t *arg_data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, arg, &mrb_leapmotion_vector_type));

  return mrb_float_value(mrb, data->obj->distanceTo(*arg_data->obj));
}

static mrb_value
mrb_leapmotion_vector_angle_to(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  mrb_data_check_type(mrb, arg, &mrb_leapmotion_vector_type);

  mrb_leapmotion_vector_t *arg_data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, arg, &mrb_leapmotion_vector_type));

  return mrb_float_value(mrb, data->obj->angleTo(*arg_data->obj));
}

static mrb_value
mrb_leapmotion_vector_pitch(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->pitch());
}

static mrb_value
mrb_leapmotion_vector_yaw(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->yaw());
}

static mrb_value
mrb_leapmotion_vector_roll(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->roll());
}

static mrb_value
mrb_leapmotion_vector_dot(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  mrb_data_check_type(mrb, arg, &mrb_leapmotion_vector_type);

  mrb_leapmotion_vector_t *arg_data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, arg, &mrb_leapmotion_vector_type));

  return mrb_float_value(mrb, data->obj->dot(*arg_data->obj));
}

static mrb_value
mrb_leapmotion_vector_cross(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  mrb_data_check_type(mrb, arg, &mrb_leapmotion_vector_type);

  mrb_leapmotion_vector_t *arg_data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, arg, &mrb_leapmotion_vector_type));

  return mrb_leapmotion_obj_make(mrb, data->obj->cross(*arg_data->obj));
}

static mrb_value
mrb_leapmotion_vector_normalized(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->normalized());
}

static mrb_value
mrb_leapmotion_vector_plus(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  mrb_data_check_type(mrb, arg, &mrb_leapmotion_vector_type);

  mrb_leapmotion_vector_t *arg_data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, arg, &mrb_leapmotion_vector_type));

  return mrb_leapmotion_obj_make(mrb, *data->obj + *arg_data->obj);
}

static mrb_value
mrb_leapmotion_vector_uminus(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, -*data->obj);
}

static mrb_value
mrb_leapmotion_vector_minus(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  mrb_data_check_type(mrb, arg, &mrb_leapmotion_vector_type);

  mrb_leapmotion_vector_t *arg_data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, arg, &mrb_leapmotion_vector_type));

  return mrb_leapmotion_obj_make(mrb, *data->obj - *arg_data->obj);
}

static mrb_value
mrb_leapmotion_vector_multiple(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_float arg;
  mrb_get_args(mrb, "f", &arg);

  return mrb_leapmotion_obj_make(mrb, *data->obj * arg);
}

static mrb_value
mrb_leapmotion_vector_divide(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_float arg;
  mrb_get_args(mrb, "f", &arg);

  return mrb_leapmotion_obj_make(mrb, *data->obj / arg);
}

static mrb_value
mrb_leapmotion_vector_equal(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  mrb_data_check_type(mrb, arg, &mrb_leapmotion_vector_type);

  mrb_leapmotion_vector_t *arg_data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, arg, &mrb_leapmotion_vector_type));

  return (*data->obj == *arg_data->obj) ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_vector_is_valid(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->isValid() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_vector_get_at(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
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
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  std::string const str(std::move(data->obj->toString()));
  return mrb_str_new(mrb, str.c_str(), str.length());
}

static mrb_value
mrb_leapmotion_vector_get_x(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->x);
}

static mrb_value
mrb_leapmotion_vector_get_y(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->y);
}

static mrb_value
mrb_leapmotion_vector_get_z(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_vector_t *data =
    static_cast<mrb_leapmotion_vector_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_vector_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->z);
}

/*
 * LeapMotion::Vector class methods
 */

static mrb_value
mrb_leapmotion_vector_zero(mrb_state *mrb, mrb_value self)
{
  return mrb_leapmotion_obj_make(mrb, std::move(Leap::Vector::zero()));
}

static mrb_value
mrb_leapmotion_vector_x_axis(mrb_state *mrb, mrb_value self)
{
  return mrb_leapmotion_obj_make(mrb, std::move(Leap::Vector::xAxis()));
}

static mrb_value
mrb_leapmotion_vector_y_axis(mrb_state *mrb, mrb_value self)
{
  return mrb_leapmotion_obj_make(mrb, std::move(Leap::Vector::yAxis()));
}

static mrb_value
mrb_leapmotion_vector_z_axis(mrb_state *mrb, mrb_value self)
{
  return mrb_leapmotion_obj_make(mrb, std::move(Leap::Vector::zAxis()));
}

static mrb_value
mrb_leapmotion_vector_left(mrb_state *mrb, mrb_value self)
{
  return mrb_leapmotion_obj_make(mrb, std::move(Leap::Vector::left()));
}

static mrb_value
mrb_leapmotion_vector_right(mrb_state *mrb, mrb_value self)
{
  return mrb_leapmotion_obj_make(mrb, std::move(Leap::Vector::right()));
}

static mrb_value
mrb_leapmotion_vector_down(mrb_state *mrb, mrb_value self)
{
  return mrb_leapmotion_obj_make(mrb, std::move(Leap::Vector::down()));
}

static mrb_value
mrb_leapmotion_vector_up(mrb_state *mrb, mrb_value self)
{
  return mrb_leapmotion_obj_make(mrb, std::move(Leap::Vector::up()));
}

static mrb_value
mrb_leapmotion_vector_forward(mrb_state *mrb, mrb_value self)
{
  return mrb_leapmotion_obj_make(mrb, std::move(Leap::Vector::forward()));
}

static mrb_value
mrb_leapmotion_vector_backward(mrb_state *mrb, mrb_value self)
{
  return mrb_leapmotion_obj_make(mrb, std::move(Leap::Vector::backward()));
}

/*
 * LeapMotion::Frame instance methods
 */

static mrb_value
mrb_leapmotion_frame_id(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, (mrb_float)data->obj->id());
}

static mrb_value
mrb_leapmotion_frame_timestamp(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, (mrb_float)data->obj->timestamp());
}

static mrb_value
mrb_leapmotion_frame_hands(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->hands());
}

static mrb_value
mrb_leapmotion_frame_hand(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  return mrb_leapmotion_obj_make(mrb, data->obj->hand(index));
}

static mrb_value
mrb_leapmotion_frame_pointables(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->pointables());
}

static mrb_value
mrb_leapmotion_frame_pointable(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_int id;
  mrb_get_args(mrb, "i", &id);
  return mrb_leapmotion_obj_make(mrb, data->obj->pointable(id));
}

static mrb_value
mrb_leapmotion_frame_fingers(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->fingers());
}

static mrb_value
mrb_leapmotion_frame_finger(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_int id;
  mrb_get_args(mrb, "i", &id);
  return mrb_leapmotion_obj_make(mrb, data->obj->finger(id));
}

static mrb_value
mrb_leapmotion_frame_tools(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->tools());
}

static mrb_value
mrb_leapmotion_frame_tool(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_int id;
  mrb_get_args(mrb, "i", &id);
  return mrb_leapmotion_obj_make(mrb, data->obj->tool(id));
}

static mrb_value
mrb_leapmotion_frame_gestures(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->gestures());
}

static mrb_value
mrb_leapmotion_frame_gesture(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  return mrb_leapmotion_obj_make(mrb, data->obj->gesture(index));
}

static mrb_value
mrb_leapmotion_frame_translation(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value frame;
  mrb_get_args(mrb, "o", &frame);
  mrb_data_check_type(mrb, frame, &mrb_leapmotion_frame_type);
  mrb_leapmotion_frame_t *fdata = static_cast<mrb_leapmotion_frame_t*>(DATA_PTR(frame));
  return mrb_leapmotion_obj_make(mrb, data->obj->translation(*fdata->obj));
}

static mrb_value
mrb_leapmotion_frame_translation_probability(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value frame;
  mrb_get_args(mrb, "o", &frame);
  mrb_data_check_type(mrb, frame, &mrb_leapmotion_frame_type);
  mrb_leapmotion_frame_t *fdata = static_cast<mrb_leapmotion_frame_t*>(DATA_PTR(frame));
  return mrb_float_value(mrb, data->obj->translationProbability(*fdata->obj));
}

static mrb_value
mrb_leapmotion_frame_rotation_axis(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value frame;
  mrb_get_args(mrb, "o", &frame);
  mrb_data_check_type(mrb, frame, &mrb_leapmotion_frame_type);
  mrb_leapmotion_frame_t *fdata = static_cast<mrb_leapmotion_frame_t*>(DATA_PTR(frame));
  return mrb_leapmotion_obj_make(mrb, data->obj->rotationAxis(*fdata->obj));
}

static mrb_value
mrb_leapmotion_frame_rotation_angle(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value frame, axis;
  int const argc = mrb_get_args(mrb, "o|o", &frame, &axis);
  mrb_data_check_type(mrb, frame, &mrb_leapmotion_frame_type);
  if (argc > 1) {
    mrb_data_check_type(mrb, axis, &mrb_leapmotion_vector_type);
  }
  mrb_leapmotion_frame_t *fdata = static_cast<mrb_leapmotion_frame_t*>(DATA_PTR(frame));
  if (argc == 1) {
    return mrb_float_value(mrb, data->obj->rotationAngle(*fdata->obj));
  } else if (argc > 1) {
    mrb_leapmotion_vector_t *adata = static_cast<mrb_leapmotion_vector_t*>(DATA_PTR(axis));
    return mrb_float_value(mrb, data->obj->rotationAngle(*fdata->obj, *adata->obj));
  }
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_frame_rotation_matrix(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_frame_rotation_probability(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value frame;
  mrb_get_args(mrb, "o", &frame);
  mrb_data_check_type(mrb, frame, &mrb_leapmotion_frame_type);
  mrb_leapmotion_frame_t *fdata = static_cast<mrb_leapmotion_frame_t*>(DATA_PTR(frame));
  return mrb_float_value(mrb, data->obj->rotationProbability(*fdata->obj));
}

static mrb_value
mrb_leapmotion_frame_scale_factor(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value frame;
  mrb_get_args(mrb, "o", &frame);
  mrb_data_check_type(mrb, frame, &mrb_leapmotion_frame_type);
  mrb_leapmotion_frame_t *fdata = static_cast<mrb_leapmotion_frame_t*>(DATA_PTR(frame));
  return mrb_float_value(mrb, data->obj->scaleFactor(*fdata->obj));
}

static mrb_value
mrb_leapmotion_frame_scale_probability(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value frame;
  mrb_get_args(mrb, "o", &frame);
  mrb_data_check_type(mrb, frame, &mrb_leapmotion_frame_type);
  mrb_leapmotion_frame_t *fdata = static_cast<mrb_leapmotion_frame_t*>(DATA_PTR(frame));
  return mrb_float_value(mrb, data->obj->scaleProbability(*fdata->obj));
}

static mrb_value
mrb_leapmotion_frame_interaction_box(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_frame_is_valid(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->isValid() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_frame_to_s(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  std::string const str(std::move(data->obj->toString()));
  return mrb_str_new(mrb, str.c_str(), str.length());
}

static mrb_value
mrb_leapmotion_frame_equal(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_frame_t *data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_frame_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  mrb_data_check_type(mrb, arg, &mrb_leapmotion_frame_type);

  mrb_leapmotion_frame_t *arg_data =
    static_cast<mrb_leapmotion_frame_t*>(mrb_data_get_ptr(mrb, arg, &mrb_leapmotion_frame_type));
  return (*data->obj == *arg_data->obj) ? mrb_true_value() : mrb_false_value();
}

/*
 * LeapMotion::Frame class methods
 */

static mrb_value
mrb_leapmotion_frame_invalid(mrb_state *mrb, mrb_value self)
{
  return mrb_leapmotion_obj_make(mrb, std::move(Leap::Frame::invalid()));
}

/*
 * LeapMotion::HandList
 */

static mrb_value
mrb_leapmotion_handlist_count(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_handlist_t *data =
    static_cast<mrb_leapmotion_handlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_handlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_fixnum_value(data->obj->count());
}

static mrb_value
mrb_leapmotion_handlist_is_empty(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_handlist_t *data =
    static_cast<mrb_leapmotion_handlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_handlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->empty() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_handlist_get_at(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_handlist_t *data =
    static_cast<mrb_leapmotion_handlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_handlist_type));
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
  mrb_leapmotion_handlist_t *data =
    static_cast<mrb_leapmotion_handlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_handlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->leftmost());
}

static mrb_value
mrb_leapmotion_handlist_rightmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_handlist_t *data =
    static_cast<mrb_leapmotion_handlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_handlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->rightmost());
}

static mrb_value
mrb_leapmotion_handlist_frontmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_handlist_t *data =
    static_cast<mrb_leapmotion_handlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_handlist_type));
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
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->id());
}

static mrb_value
mrb_leapmotion_hand_frame(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->frame());
}

static mrb_value
mrb_leapmotion_hand_pointables(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->pointables());
}

static mrb_value
mrb_leapmotion_hand_pointable(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_int id;
  mrb_get_args(mrb, "i", &id);
  return mrb_leapmotion_obj_make(mrb, data->obj->pointable(id));
}

static mrb_value
mrb_leapmotion_hand_fingers(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->fingers());
}

static mrb_value
mrb_leapmotion_hand_finger(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_int id;
  mrb_get_args(mrb, "i", &id);
  return mrb_leapmotion_obj_make(mrb, data->obj->finger(id));
}

static mrb_value
mrb_leapmotion_hand_tools(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->tools());
}

static mrb_value
mrb_leapmotion_hand_tool(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_int id;
  mrb_get_args(mrb, "i", &id);
  return mrb_leapmotion_obj_make(mrb, data->obj->tool(id));
}

static mrb_value
mrb_leapmotion_hand_palm_position(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->palmPosition());
}

static mrb_value
mrb_leapmotion_hand_palm_velocity(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->palmVelocity());
}

static mrb_value
mrb_leapmotion_hand_palm_normal(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->palmNormal());
}

static mrb_value
mrb_leapmotion_hand_direction(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->direction());
}

static mrb_value
mrb_leapmotion_hand_sphere_center(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->sphereCenter());
}

static mrb_value
mrb_leapmotion_hand_sphere_radius(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->sphereRadius());
}

static mrb_value
mrb_leapmotion_hand_translation(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value frame;
  mrb_get_args(mrb, "o", &frame);
  mrb_data_check_type(mrb, frame, &mrb_leapmotion_frame_type);
  mrb_leapmotion_frame_t *fdata = static_cast<mrb_leapmotion_frame_t*>(DATA_PTR(frame));
  return mrb_leapmotion_obj_make(mrb, data->obj->translation(*fdata->obj));
}

static mrb_value
mrb_leapmotion_hand_translation_probability(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value frame;
  mrb_get_args(mrb, "o", &frame);
  mrb_data_check_type(mrb, frame, &mrb_leapmotion_frame_type);
  mrb_leapmotion_frame_t *fdata = static_cast<mrb_leapmotion_frame_t*>(DATA_PTR(frame));
  return mrb_float_value(mrb, data->obj->translationProbability(*fdata->obj));
}

static mrb_value
mrb_leapmotion_hand_rotation_axis(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value frame;
  mrb_get_args(mrb, "o", &frame);
  mrb_data_check_type(mrb, frame, &mrb_leapmotion_frame_type);
  mrb_leapmotion_frame_t *fdata = static_cast<mrb_leapmotion_frame_t*>(DATA_PTR(frame));
  return mrb_leapmotion_obj_make(mrb, data->obj->rotationAxis(*fdata->obj));
}

static mrb_value
mrb_leapmotion_hand_rotation_angle(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value frame, axis;
  int const argc = mrb_get_args(mrb, "o|o", &frame, &axis);
  mrb_data_check_type(mrb, frame, &mrb_leapmotion_frame_type);
  if (argc > 1) {
    mrb_data_check_type(mrb, axis, &mrb_leapmotion_vector_type);
  }
  mrb_leapmotion_frame_t *fdata = static_cast<mrb_leapmotion_frame_t*>(DATA_PTR(frame));
  if (argc == 1) {
    return mrb_float_value(mrb, data->obj->rotationAngle(*fdata->obj));
  } else if (argc > 1) {
    mrb_leapmotion_vector_t *adata = static_cast<mrb_leapmotion_vector_t*>(DATA_PTR(axis));
    return mrb_float_value(mrb, data->obj->rotationAngle(*fdata->obj, *adata->obj));
  }
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_hand_rotation_matrix(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_hand_rotation_probability(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value frame;
  mrb_get_args(mrb, "o", &frame);
  mrb_data_check_type(mrb, frame, &mrb_leapmotion_frame_type);
  mrb_leapmotion_frame_t *fdata = static_cast<mrb_leapmotion_frame_t*>(DATA_PTR(frame));
  return mrb_float_value(mrb, data->obj->rotationProbability(*fdata->obj));
}

static mrb_value
mrb_leapmotion_hand_scale_factor(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value frame;
  mrb_get_args(mrb, "o", &frame);
  mrb_data_check_type(mrb, frame, &mrb_leapmotion_frame_type);
  mrb_leapmotion_frame_t *fdata = static_cast<mrb_leapmotion_frame_t*>(DATA_PTR(frame));
  return mrb_float_value(mrb, data->obj->scaleFactor(*fdata->obj));
}

static mrb_value
mrb_leapmotion_hand_scale_probability(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value frame;
  mrb_get_args(mrb, "o", &frame);
  mrb_data_check_type(mrb, frame, &mrb_leapmotion_frame_type);
  mrb_leapmotion_frame_t *fdata = static_cast<mrb_leapmotion_frame_t*>(DATA_PTR(frame));
  return mrb_float_value(mrb, data->obj->scaleProbability(*fdata->obj));
}

static mrb_value
mrb_leapmotion_hand_is_valid(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->isValid() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_hand_equal(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value rhs;
  mrb_get_args(mrb, "o", &rhs);
  mrb_data_check_type(mrb, rhs, &mrb_leapmotion_hand_type);
  mrb_leapmotion_hand_t* rhs_data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, rhs, &mrb_leapmotion_hand_type));

  return (*data->obj == *rhs_data->obj) ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_hand_to_s(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_hand_t *data =
    static_cast<mrb_leapmotion_hand_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_hand_type));
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
  mrb_leapmotion_fingerlist_t *data =
    static_cast<mrb_leapmotion_fingerlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_fingerlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_fixnum_value(data->obj->count());
}

static mrb_value
mrb_leapmotion_fingerlist_is_empty(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_fingerlist_t *data =
    static_cast<mrb_leapmotion_fingerlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_fingerlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->empty() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_fingerlist_get_at(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_fingerlist_t *data =
    static_cast<mrb_leapmotion_fingerlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_fingerlist_type));
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
  mrb_leapmotion_fingerlist_t *data =
    static_cast<mrb_leapmotion_fingerlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_fingerlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->leftmost());
}

static mrb_value
mrb_leapmotion_fingerlist_rightmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_fingerlist_t *data =
    static_cast<mrb_leapmotion_fingerlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_fingerlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->rightmost());
}

static mrb_value
mrb_leapmotion_fingerlist_frontmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_fingerlist_t *data =
    static_cast<mrb_leapmotion_fingerlist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_fingerlist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->frontmost());
}

/*
 * LeapMotion::PointableList
 */

static mrb_value
mrb_leapmotion_pointablelist_count(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointablelist_t *data =
    static_cast<mrb_leapmotion_pointablelist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointablelist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_fixnum_value(data->obj->count());
}

static mrb_value
mrb_leapmotion_pointablelist_is_empty(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointablelist_t *data =
    static_cast<mrb_leapmotion_pointablelist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointablelist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->empty() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_pointablelist_get_at(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointablelist_t *data =
    static_cast<mrb_leapmotion_pointablelist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointablelist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  return mrb_leapmotion_obj_make(mrb, data->obj->operator[](index));
}

static mrb_value
mrb_leapmotion_pointablelist_append(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointablelist_t *data =
    static_cast<mrb_leapmotion_pointablelist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointablelist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value list;
  mrb_get_args(mrb, "o", &list);
  mrb_leapmotion_pointablelist_t *plist =
    static_cast<mrb_leapmotion_pointablelist_t*>(mrb_data_get_ptr(mrb, list, &mrb_leapmotion_pointablelist_type));
  if (plist != NULL) {
    return mrb_leapmotion_obj_make(mrb, data->obj->append(*plist->obj));
  }
  mrb_leapmotion_fingerlist_t *flist =
    static_cast<mrb_leapmotion_fingerlist_t*>(mrb_data_get_ptr(mrb, list, &mrb_leapmotion_fingerlist_type));
  if (flist != NULL) {
    return mrb_leapmotion_obj_make(mrb, data->obj->append(*flist->obj));
  }
  mrb_leapmotion_toollist_t *tlist =
    static_cast<mrb_leapmotion_toollist_t*>(mrb_data_get_ptr(mrb, list, &mrb_leapmotion_toollist_type));
  if (tlist != NULL) {
    return mrb_leapmotion_obj_make(mrb, data->obj->append(*tlist->obj));
  }
  mrb_raise(mrb, E_ARGUMENT_ERROR, "given argument is not a list of pointable types.");
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_pointablelist_leftmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointablelist_t *data =
    static_cast<mrb_leapmotion_pointablelist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointablelist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->leftmost());
}

static mrb_value
mrb_leapmotion_pointablelist_rightmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointablelist_t *data =
    static_cast<mrb_leapmotion_pointablelist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointablelist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->rightmost());
}

static mrb_value
mrb_leapmotion_pointablelist_frontmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointablelist_t *data =
    static_cast<mrb_leapmotion_pointablelist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointablelist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->frontmost());
}

/*
 * LeapMotion::Pointable
 */

static mrb_leapmotion_pointable_t*
get_pointable_data_ptr(mrb_state *mrb, mrb_value self)
{
  if (mrb_type(self) != MRB_TT_DATA) {
    return NULL;
  }
  mrb_data_type const * const type = DATA_TYPE(self);
  if ((type == &mrb_leapmotion_pointable_type) ||
      (type == &mrb_leapmotion_finger_type) ||
      (type == &mrb_leapmotion_tool_type)) {
    return static_cast<mrb_leapmotion_pointable_t*>(DATA_PTR(self));
  }
  return NULL;
}

static mrb_value
mrb_leapmotion_pointable_id(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data =
    static_cast<mrb_leapmotion_pointable_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointable_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->id());
}

static mrb_value
mrb_leapmotion_pointable_frame(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data =
    static_cast<mrb_leapmotion_pointable_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_pointable_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->frame());
}

static mrb_value
mrb_leapmotion_pointable_hand(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = get_pointable_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->hand());
}

static mrb_value
mrb_leapmotion_pointable_tip_position(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = get_pointable_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->tipPosition());
}

static mrb_value
mrb_leapmotion_pointable_tip_velocity(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = get_pointable_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->tipVelocity());
}

static mrb_value
mrb_leapmotion_pointable_direction(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = get_pointable_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->direction());
}

static mrb_value
mrb_leapmotion_pointable_width(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = get_pointable_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->width());
}

static mrb_value
mrb_leapmotion_pointable_length(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = get_pointable_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->length());
}

static mrb_value
mrb_leapmotion_pointable_is_finger(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = get_pointable_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->isFinger() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_pointable_is_tool(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = get_pointable_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->isTool() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_pointable_is_valid(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = get_pointable_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->isValid() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_pointable_touch_zone(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_leapmotion_pointable_touch_distance(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = get_pointable_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->touchDistance());
}

static mrb_value
mrb_leapmotion_pointable_stabilized_tip_position(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = get_pointable_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->stabilizedTipPosition());
}

static mrb_value
mrb_leapmotion_pointable_equal(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = get_pointable_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value rhs;
  mrb_get_args(mrb, "o", &rhs);
  if (mrb_type(rhs) != MRB_TT_DATA) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "argument type mismatch.");
  }
  mrb_data_type const * const type = DATA_TYPE(rhs);
  if ((type == &mrb_leapmotion_pointable_type) ||
      (type == &mrb_leapmotion_finger_type) ||
      (type == &mrb_leapmotion_tool_type)) {
      return (*data->obj == *static_cast<mrb_leapmotion_pointable_t*>(DATA_PTR(rhs))->obj) ?
        mrb_true_value() : mrb_false_value();
  }
  mrb_raise(mrb, E_ARGUMENT_ERROR, "given argument is not pointable type.");
  return mrb_false_value();
}

static mrb_value
mrb_leapmotion_pointable_to_s(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_pointable_t *data = get_pointable_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  std::string const str(std::move(data->obj->toString()));
  return mrb_str_new(mrb, str.c_str(), str.length());
}

/*
 * LeapMotion::Finger instance methods
 */

static mrb_value
mrb_leapmotion_finger_to_s(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_finger_t *data =
    static_cast<mrb_leapmotion_finger_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_finger_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  std::string const str(std::move(data->obj->toString()));
  return mrb_str_new(mrb, str.c_str(), str.length());
}

/*
 * LeapMotion::Finger class methods
 */

static mrb_value
mrb_leapmotion_finger_invalid(mrb_state *mrb, mrb_value self)
{
  return mrb_leapmotion_obj_make(mrb, std::move(Leap::Finger::invalid()));
}

/*
 * LeapMotion::ToolList instance methods
 */

static mrb_value
mrb_leapmotion_toollist_count(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_toollist_t *data =
    static_cast<mrb_leapmotion_toollist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_toollist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_fixnum_value(data->obj->count());
}

static mrb_value
mrb_leapmotion_toollist_is_empty(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_toollist_t *data =
    static_cast<mrb_leapmotion_toollist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_toollist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->empty() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_toollist_get_at(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_toollist_t *data =
    static_cast<mrb_leapmotion_toollist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_toollist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  return mrb_leapmotion_obj_make(mrb, data->obj->operator[](index));
}

static mrb_value
mrb_leapmotion_toollist_append(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_toollist_t *data =
    static_cast<mrb_leapmotion_toollist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_toollist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }

  mrb_value list;
  mrb_get_args(mrb, "o", &list);
  mrb_data_check_type(mrb, list, &mrb_leapmotion_toollist_type);

  mrb_leapmotion_toollist_t *ldata =
    static_cast<mrb_leapmotion_toollist_t*>(DATA_PTR(list));

  return mrb_leapmotion_obj_make(mrb, data->obj->append(*ldata->obj));
}

static mrb_value
mrb_leapmotion_toollist_leftmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_toollist_t *data =
    static_cast<mrb_leapmotion_toollist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_toollist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->leftmost());
}

static mrb_value
mrb_leapmotion_toollist_rightmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_toollist_t *data =
    static_cast<mrb_leapmotion_toollist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_toollist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->rightmost());
}

static mrb_value
mrb_leapmotion_toollist_frontmost(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_toollist_t *data =
    static_cast<mrb_leapmotion_toollist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_toollist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->frontmost());
}

/*
 * LeapMotion::Tool instance methods
 */

static mrb_value
mrb_leapmotion_tool_to_s(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_tool_t *data =
    static_cast<mrb_leapmotion_tool_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_tool_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  std::string const str(std::move(data->obj->toString()));
  return mrb_str_new(mrb, str.c_str(), str.length());
}

/*
 * LeapMotion::Tool class methods
 */

static mrb_value
mrb_leapmotion_tool_invalid(mrb_state *mrb, mrb_value self)
{
  return mrb_leapmotion_obj_make(mrb, std::move(Leap::Tool::invalid()));
}

/*
 * LeapMotion::GestureList
 */

static mrb_value
mrb_leapmotion_gesturelist_count(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_gesturelist_t *data =
    static_cast<mrb_leapmotion_gesturelist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_gesturelist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_fixnum_value(data->obj->count());
}

static mrb_value
mrb_leapmotion_gesturelist_is_empty(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_gesturelist_t *data =
    static_cast<mrb_leapmotion_gesturelist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_gesturelist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->empty() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_gesturelist_get_at(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_gesturelist_t *data =
    static_cast<mrb_leapmotion_gesturelist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_gesturelist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  return mrb_leapmotion_obj_make(mrb, data->obj->operator[](index));
}

static mrb_value
mrb_leapmotion_gesturelist_append(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_gesturelist_t *data =
    static_cast<mrb_leapmotion_gesturelist_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_gesturelist_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  mrb_value arg;
  mrb_get_args(mrb, "o", &arg);
  mrb_data_check_type(mrb, arg, &mrb_leapmotion_gesturelist_type);
  mrb_leapmotion_gesturelist_t *arg_data =
    static_cast<mrb_leapmotion_gesturelist_t*>(mrb_data_get_ptr(mrb, arg, &mrb_leapmotion_gesturelist_type));
  return mrb_leapmotion_obj_make(mrb, data->obj->append(*arg_data->obj));
}

/*
 * LeapMotion::Gesture
 */

static mrb_leapmotion_gesture_t*
get_gesture_data_ptr(mrb_state *mrb, mrb_value self)
{
  if (mrb_type(self) != MRB_TT_DATA) {
    return NULL;
  }
  mrb_data_type const * const type = DATA_TYPE(self);
  if ((type == &mrb_leapmotion_gesture_type) ||
      (type == &mrb_leapmotion_swipegesture_type) ||
      (type == &mrb_leapmotion_circlegesture_type) ||
      (type == &mrb_leapmotion_screentapgesture_type) ||
      (type == &mrb_leapmotion_keytapgesture_type)) {
    return static_cast<mrb_leapmotion_gesture_t*>(DATA_PTR(self));
  }
  return NULL;
}

static mrb_value
mrb_leapmotion_gesture_get_type(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_gesture_t *data = get_gesture_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_fixnum_value(data->obj->type());
}

static mrb_value
mrb_leapmotion_gesture_state(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_gesture_t *data = get_gesture_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_fixnum_value(data->obj->state());
}

static mrb_value
mrb_leapmotion_gesture_id(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_gesture_t *data = get_gesture_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_fixnum_value(data->obj->id());
}

static mrb_value
mrb_leapmotion_gesture_duration(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_gesture_t *data = get_gesture_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->duration());
}

static mrb_value
mrb_leapmotion_gesture_duration_seconds(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_gesture_t *data = get_gesture_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, data->obj->durationSeconds());
}

static mrb_value
mrb_leapmotion_gesture_frame(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_gesture_t *data = get_gesture_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->frame());
}

static mrb_value
mrb_leapmotion_gesture_hands(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_gesture_t *data = get_gesture_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, data->obj->hands());
}

static mrb_value
mrb_leapmotion_gesture_is_valid(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_gesture_t *data = get_gesture_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  return data->obj->isValid() ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_leapmotion_gesture_to_s(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_gesture_t *data = get_gesture_data_ptr(mrb, self);
  if (data == NULL) {
    return mrb_nil_value();
  }
  std::string const str(std::move(data->obj->toString()));
  return mrb_str_new(mrb, str.c_str(), str.length());
}

/*
 * LeapMotion::SwipeGesture
 */

static mrb_value
mrb_leapmotion_swipegesture_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_swipegesture_t *data =
    static_cast<mrb_leapmotion_swipegesture_t*>(DATA_PTR(self));

  if (data != NULL) {
    mrb_leapmotion_swipegesture_free(mrb, data);
  }

  mrb_value gesture;
  mrb_get_args(mrb, "o", &gesture);
  mrb_data_check_type(mrb, gesture, &mrb_leapmotion_gesture_type);

  mrb_leapmotion_gesture_t * const gdata =
    static_cast<mrb_leapmotion_gesture_t*>(DATA_PTR(gesture));

  DATA_TYPE(self) = &mrb_leapmotion_swipegesture_type;
  DATA_PTR(self) = mrb_leapmotion_gesture_alloc<Leap::SwipeGesture>(mrb, gdata);

  return self;
}

static mrb_value
mrb_leapmotion_swipegesture_start_position(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_swipegesture_t *data =
    static_cast<mrb_leapmotion_swipegesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_swipegesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, static_cast<Leap::SwipeGesture*>(data->obj)->startPosition());
}

static mrb_value
mrb_leapmotion_swipegesture_position(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_swipegesture_t *data =
    static_cast<mrb_leapmotion_swipegesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_swipegesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, static_cast<Leap::SwipeGesture*>(data->obj)->position());
}

static mrb_value
mrb_leapmotion_swipegesture_direction(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_swipegesture_t *data =
    static_cast<mrb_leapmotion_swipegesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_swipegesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, static_cast<Leap::SwipeGesture*>(data->obj)->direction());
}

static mrb_value
mrb_leapmotion_swipegesture_speed(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_swipegesture_t *data =
    static_cast<mrb_leapmotion_swipegesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_swipegesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, static_cast<Leap::SwipeGesture*>(data->obj)->speed());
}

static mrb_value
mrb_leapmotion_swipegesture_pointable(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_swipegesture_t *data =
    static_cast<mrb_leapmotion_swipegesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_swipegesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, static_cast<Leap::SwipeGesture*>(data->obj)->pointable());
}

/*
 * LeapMotion::CircleGesture
 */

static mrb_value
mrb_leapmotion_circlegesture_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_circlegesture_t *data =
    static_cast<mrb_leapmotion_circlegesture_t*>(DATA_PTR(self));

  if (data != NULL) {
    mrb_leapmotion_circlegesture_free(mrb, data);
  }

  mrb_value gesture;
  mrb_get_args(mrb, "o", &gesture);
  mrb_data_check_type(mrb, gesture, &mrb_leapmotion_gesture_type);

  mrb_leapmotion_gesture_t * const gdata = static_cast<mrb_leapmotion_gesture_t*>(DATA_PTR(gesture));

  DATA_TYPE(self) = &mrb_leapmotion_circlegesture_type;
  DATA_PTR(self) = mrb_leapmotion_gesture_alloc<Leap::CircleGesture>(mrb, gdata);

  return self;
}

static mrb_value
mrb_leapmotion_circlegesture_center(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_circlegesture_t *data =
    static_cast<mrb_leapmotion_circlegesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_circlegesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, static_cast<Leap::CircleGesture*>(data->obj)->center());
}

static mrb_value
mrb_leapmotion_circlegesture_normal(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_circlegesture_t *data =
    static_cast<mrb_leapmotion_circlegesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_circlegesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, static_cast<Leap::CircleGesture*>(data->obj)->normal());
}

static mrb_value
mrb_leapmotion_circlegesture_progress(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_circlegesture_t *data =
    static_cast<mrb_leapmotion_circlegesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_circlegesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, static_cast<Leap::CircleGesture*>(data->obj)->progress());
}

static mrb_value
mrb_leapmotion_circlegesture_radius(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_circlegesture_t *data =
    static_cast<mrb_leapmotion_circlegesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_circlegesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, static_cast<Leap::CircleGesture*>(data->obj)->radius());
}

static mrb_value
mrb_leapmotion_circlegesture_pointable(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_circlegesture_t *data =
    static_cast<mrb_leapmotion_circlegesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_circlegesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, static_cast<Leap::CircleGesture*>(data->obj)->pointable());
}

/*
 * LeapMotion::ScreenTapGesture
 */

static mrb_value
mrb_leapmotion_screentapgesture_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_screentapgesture_t *data =
    static_cast<mrb_leapmotion_screentapgesture_t*>(DATA_PTR(self));

  if (data != NULL) {
    mrb_leapmotion_screentapgesture_free(mrb, data);
  }

  mrb_value gesture;
  mrb_get_args(mrb, "o", &gesture);
  mrb_data_check_type(mrb, gesture, &mrb_leapmotion_gesture_type);

  mrb_leapmotion_gesture_t * const gdata =
    static_cast<mrb_leapmotion_gesture_t*>(DATA_PTR(gesture));

  DATA_TYPE(self) = &mrb_leapmotion_screentapgesture_type;
  DATA_PTR(self) = mrb_leapmotion_gesture_alloc<Leap::ScreenTapGesture>(mrb, gdata);

  return self;
}

static mrb_value
mrb_leapmotion_screentapgesture_position(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_screentapgesture_t *data =
    static_cast<mrb_leapmotion_screentapgesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_screentapgesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, static_cast<Leap::ScreenTapGesture*>(data->obj)->position());
}

static mrb_value
mrb_leapmotion_screentapgesture_direction(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_screentapgesture_t *data =
    static_cast<mrb_leapmotion_screentapgesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_screentapgesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, static_cast<Leap::ScreenTapGesture*>(data->obj)->direction());
}

static mrb_value
mrb_leapmotion_screentapgesture_progress(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_screentapgesture_t *data =
    static_cast<mrb_leapmotion_screentapgesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_screentapgesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, static_cast<Leap::ScreenTapGesture*>(data->obj)->progress());
}

static mrb_value
mrb_leapmotion_screentapgesture_pointable(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_screentapgesture_t *data =
    static_cast<mrb_leapmotion_screentapgesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_screentapgesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, static_cast<Leap::ScreenTapGesture*>(data->obj)->pointable());
}

/*
 * LeapMotion::KeyTapGesture
 */

static mrb_value
mrb_leapmotion_keytapgesture_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_keytapgesture_t *data =
    static_cast<mrb_leapmotion_keytapgesture_t*>(DATA_PTR(self));

  if (data != NULL) {
    mrb_leapmotion_keytapgesture_free(mrb, data);
  }

  mrb_value gesture;
  mrb_get_args(mrb, "o", &gesture);
  mrb_data_check_type(mrb, gesture, &mrb_leapmotion_gesture_type);

  mrb_leapmotion_gesture_t * const gdata =
    static_cast<mrb_leapmotion_gesture_t*>(DATA_PTR(gesture));

  DATA_TYPE(self) = &mrb_leapmotion_keytapgesture_type;
  DATA_PTR(self) = mrb_leapmotion_gesture_alloc<Leap::KeyTapGesture>(mrb, gdata);

  return self;
}

static mrb_value
mrb_leapmotion_keytapgesture_position(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_keytapgesture_t *data =
    static_cast<mrb_leapmotion_keytapgesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_keytapgesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, static_cast<Leap::KeyTapGesture*>(data->obj)->position());
}

static mrb_value
mrb_leapmotion_keytapgesture_direction(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_keytapgesture_t *data =
    static_cast<mrb_leapmotion_keytapgesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_keytapgesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, static_cast<Leap::KeyTapGesture*>(data->obj)->direction());
}

static mrb_value
mrb_leapmotion_keytapgesture_progress(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_keytapgesture_t *data =
    static_cast<mrb_leapmotion_keytapgesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_keytapgesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_float_value(mrb, static_cast<Leap::KeyTapGesture*>(data->obj)->progress());
}

static mrb_value
mrb_leapmotion_keytapgesture_pointable(mrb_state *mrb, mrb_value self)
{
  mrb_leapmotion_keytapgesture_t *data =
    static_cast<mrb_leapmotion_keytapgesture_t*>(mrb_data_get_ptr(mrb, self, &mrb_leapmotion_keytapgesture_type));
  if (data == NULL) {
    return mrb_nil_value();
  }
  return mrb_leapmotion_obj_make(mrb, static_cast<Leap::KeyTapGesture*>(data->obj)->pointable());
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
  RClass *module_top             = mrb_define_module(mrb, "LeapMotion");
  RClass *class_controller       = mrb_define_class_under(mrb, module_top, "Controller",       mrb->object_class);
  RClass *class_config           = mrb_define_class_under(mrb, module_top, "Config",           mrb->object_class);
  RClass *class_pointablelist    = mrb_define_class_under(mrb, module_top, "PointableList",    mrb->object_class);
  RClass *class_pointable        = mrb_define_class_under(mrb, module_top, "Pointable",        mrb->object_class);
  RClass *class_toollist         = mrb_define_class_under(mrb, module_top, "ToolList",         mrb->object_class);
  RClass *class_tool             = mrb_define_class_under(mrb, module_top, "Tool",             class_pointable);
  RClass *class_devicelist       = mrb_define_class_under(mrb, module_top, "DeviceList",       mrb->object_class);
  RClass *class_fingerlist       = mrb_define_class_under(mrb, module_top, "FingerList",       mrb->object_class);
  RClass *class_finger           = mrb_define_class_under(mrb, module_top, "Finger",           class_pointable);
  RClass *class_frame            = mrb_define_class_under(mrb, module_top, "Frame",            mrb->object_class);
  RClass *class_handlist         = mrb_define_class_under(mrb, module_top, "HandList",         mrb->object_class);
  RClass *class_hand             = mrb_define_class_under(mrb, module_top, "Hand",             mrb->object_class);
  RClass *class_listener         = mrb_define_class_under(mrb, module_top, "Listener",         mrb->object_class);
  RClass *class_vector           = mrb_define_class_under(mrb, module_top, "Vector",           mrb->object_class);
  RClass *class_gesturelist      = mrb_define_class_under(mrb, module_top, "GestureList",      mrb->object_class);
  RClass *class_gesture          = mrb_define_class_under(mrb, module_top, "Gesture",          mrb->object_class);
  RClass *class_swipegesture     = mrb_define_class_under(mrb, module_top, "SwipeGesture",     class_gesture);
  RClass *class_circlegesture    = mrb_define_class_under(mrb, module_top, "CircleGesture",    class_gesture);
  RClass *class_screentapgesture = mrb_define_class_under(mrb, module_top, "ScreenTapGesture", class_gesture);
  RClass *class_keytapgesture    = mrb_define_class_under(mrb, module_top, "KeyTapGesture",    class_gesture);

  MRB_SET_INSTANCE_TT(class_controller,       MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_config,           MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_pointablelist,    MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_pointable,        MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_tool,             MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_devicelist,       MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_fingerlist,       MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_finger,           MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_frame,            MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_handlist,         MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_hand,             MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_listener,         MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_vector,           MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_gesturelist,      MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_gesture,          MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_swipegesture,     MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_circlegesture,    MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_screentapgesture, MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_keytapgesture,    MRB_TT_DATA);

  mrb_define_method(mrb, class_controller, "initialize",         mrb_leapmotion_controller_initialize,         ARGS_NONE());
  mrb_define_method(mrb, class_controller, "policy_flags",       mrb_leapmotion_controller_policy_flags,       ARGS_NONE());
  mrb_define_method(mrb, class_controller, "policy_flags=",      mrb_leapmotion_controller_set_policy_flags,   ARGS_REQ(1));
  mrb_define_method(mrb, class_controller, "add_listener",       mrb_leapmotion_controller_add_listener,       ARGS_REQ(1));
  mrb_define_method(mrb, class_controller, "remove_listener",    mrb_leapmotion_controller_remove_listener,    ARGS_REQ(1));
  mrb_define_method(mrb, class_controller, "frame",              mrb_leapmotion_controller_frame,              ARGS_REQ(1));
  mrb_define_method(mrb, class_controller, "config",             mrb_leapmotion_controller_config,             ARGS_NONE());
  mrb_define_method(mrb, class_controller, "devices",            mrb_leapmotion_controller_devices,            ARGS_NONE());
  mrb_define_method(mrb, class_controller, "located_screens",    mrb_leapmotion_controller_located_screens,    ARGS_NONE());
  mrb_define_method(mrb, class_controller, "calibrated_screens", mrb_leapmotion_controller_calibrated_screens, ARGS_NONE());
  mrb_define_method(mrb, class_controller, "enable_gesture",     mrb_leapmotion_controller_enable_gesture,     ARGS_REQ(2));
  mrb_define_method(mrb, class_controller, "gesture_enabled?",   mrb_leapmotion_controller_is_gesture_enabled, ARGS_REQ(1));
  mrb_define_method(mrb, class_controller, "connected?",         mrb_leapmotion_controller_is_connected,       ARGS_NONE());
  mrb_define_method(mrb, class_controller, "has_focus?",         mrb_leapmotion_controller_has_focus,          ARGS_NONE());

  RClass *module_policy_flag = mrb_define_module_under(mrb, class_controller, "PolicyFlag");
  mrb_define_const(mrb, module_policy_flag, "POLICY_DEFAULT",
    mrb_fixnum_value(Leap::Controller::PolicyFlag::POLICY_DEFAULT));
  mrb_define_const(mrb, module_policy_flag, "POLICY_BACKGROUND_FRAMES",
    mrb_fixnum_value(Leap::Controller::PolicyFlag::POLICY_BACKGROUND_FRAMES));

  mrb_define_method(mrb, class_config, "type",    mrb_leapmotion_config_get_type,   ARGS_REQ(1));
  mrb_define_method(mrb, class_config, "bool",    mrb_leapmotion_config_get_bool,   ARGS_REQ(1));
  mrb_define_method(mrb, class_config, "bool=",   mrb_leapmotion_config_set_bool,   ARGS_REQ(2));
  mrb_define_method(mrb, class_config, "int",     mrb_leapmotion_config_get_int,    ARGS_REQ(1));
  mrb_define_method(mrb, class_config, "int=",    mrb_leapmotion_config_set_int,    ARGS_REQ(2));
  mrb_define_method(mrb, class_config, "float",   mrb_leapmotion_config_get_float,  ARGS_REQ(1));
  mrb_define_method(mrb, class_config, "float=",  mrb_leapmotion_config_set_float,  ARGS_REQ(2));
  mrb_define_method(mrb, class_config, "string",  mrb_leapmotion_config_get_string, ARGS_REQ(1));
  mrb_define_method(mrb, class_config, "string=", mrb_leapmotion_config_set_string, ARGS_REQ(2));
  mrb_define_method(mrb, class_config, "save",    mrb_leapmotion_config_save,       ARGS_NONE());

  RClass *module_value_type = mrb_define_module_under(mrb, class_config, "ValueType");
  mrb_define_const(mrb, module_value_type, "TYPE_UNKNOWN",
    mrb_fixnum_value(Leap::Config::ValueType::TYPE_UNKNOWN));
  mrb_define_const(mrb, module_value_type, "TYPE_BOOLEAN",
    mrb_fixnum_value(Leap::Config::ValueType::TYPE_BOOLEAN));
  mrb_define_const(mrb, module_value_type, "TYPE_INT32",
    mrb_fixnum_value(Leap::Config::ValueType::TYPE_INT32));
  mrb_define_const(mrb, module_value_type, "TYPE_FLOAT",
    mrb_fixnum_value(Leap::Config::ValueType::TYPE_FLOAT));
  mrb_define_const(mrb, module_value_type, "TYPE_STRING",
    mrb_fixnum_value(Leap::Config::ValueType::TYPE_STRING));

  mrb_define_method(mrb, class_listener, "initialize",      mrb_leapmotion_listener_initialize,      ARGS_NONE());
  mrb_define_method(mrb, class_listener, "on_init",         mrb_leapmotion_listener_on_init,         ARGS_REQ(1));
  mrb_define_method(mrb, class_listener, "on_connect",      mrb_leapmotion_listener_on_connect,      ARGS_REQ(1));
  mrb_define_method(mrb, class_listener, "on_disconnect",   mrb_leapmotion_listener_on_disconnect,   ARGS_REQ(1));
  mrb_define_method(mrb, class_listener, "on_exit",         mrb_leapmotion_listener_on_exit,         ARGS_REQ(1));
  mrb_define_method(mrb, class_listener, "on_frame",        mrb_leapmotion_listener_on_frame,        ARGS_REQ(1));
  mrb_define_method(mrb, class_listener, "on_focus_gained", mrb_leapmotion_listener_on_focus_gained, ARGS_REQ(1));
  mrb_define_method(mrb, class_listener, "on_focus_lost",   mrb_leapmotion_listener_on_focus_lost,   ARGS_REQ(1));

  mrb_define_method(mrb, class_vector, "initialize",        mrb_leapmotion_vector_initialize,        ARGS_ANY());
  mrb_define_method(mrb, class_vector, "magnitude",         mrb_leapmotion_vector_magnitude,         ARGS_NONE());
  mrb_define_method(mrb, class_vector, "magnitude_squared", mrb_leapmotion_vector_magnitude_squared, ARGS_NONE());
  mrb_define_method(mrb, class_vector, "distance_to",       mrb_leapmotion_vector_distance_to,       ARGS_REQ(1));
  mrb_define_method(mrb, class_vector, "angle_to",          mrb_leapmotion_vector_angle_to,          ARGS_REQ(1));
  mrb_define_method(mrb, class_vector, "pitch",             mrb_leapmotion_vector_pitch,             ARGS_NONE());
  mrb_define_method(mrb, class_vector, "yaw",               mrb_leapmotion_vector_yaw,               ARGS_NONE());
  mrb_define_method(mrb, class_vector, "roll",              mrb_leapmotion_vector_roll,              ARGS_NONE());
  mrb_define_method(mrb, class_vector, "dot",               mrb_leapmotion_vector_dot,               ARGS_REQ(1));
  mrb_define_method(mrb, class_vector, "cross",             mrb_leapmotion_vector_cross,             ARGS_REQ(1));
  mrb_define_method(mrb, class_vector, "normalized",        mrb_leapmotion_vector_normalized,        ARGS_NONE());
  mrb_define_method(mrb, class_vector, "+",                 mrb_leapmotion_vector_plus,              ARGS_REQ(1));
  mrb_define_method(mrb, class_vector, "-@",                mrb_leapmotion_vector_uminus,            ARGS_NONE());
  mrb_define_method(mrb, class_vector, "-",                 mrb_leapmotion_vector_minus,             ARGS_REQ(1));
  mrb_define_method(mrb, class_vector, "*",                 mrb_leapmotion_vector_multiple,          ARGS_REQ(1));
  mrb_define_method(mrb, class_vector, "/",                 mrb_leapmotion_vector_divide,            ARGS_REQ(1));
  mrb_define_method(mrb, class_vector, "==",                mrb_leapmotion_vector_equal,             ARGS_REQ(1));
  mrb_define_method(mrb, class_vector, "valid?",            mrb_leapmotion_vector_is_valid,          ARGS_NONE());
  mrb_define_method(mrb, class_vector, "[]",                mrb_leapmotion_vector_get_at,            ARGS_REQ(1));
  mrb_define_method(mrb, class_vector, "to_s",              mrb_leapmotion_vector_to_s,              ARGS_NONE());
  mrb_define_method(mrb, class_vector, "x",                 mrb_leapmotion_vector_get_x,             ARGS_NONE());
  mrb_define_method(mrb, class_vector, "y",                 mrb_leapmotion_vector_get_y,             ARGS_NONE());
  mrb_define_method(mrb, class_vector, "z",                 mrb_leapmotion_vector_get_z,             ARGS_NONE());

  mrb_define_class_method(mrb, class_vector, "zero",     mrb_leapmotion_vector_zero,     ARGS_NONE());
  mrb_define_class_method(mrb, class_vector, "x_axis",   mrb_leapmotion_vector_x_axis,   ARGS_NONE());
  mrb_define_class_method(mrb, class_vector, "y_axis",   mrb_leapmotion_vector_y_axis,   ARGS_NONE());
  mrb_define_class_method(mrb, class_vector, "z_axis",   mrb_leapmotion_vector_z_axis,   ARGS_NONE());
  mrb_define_class_method(mrb, class_vector, "left",     mrb_leapmotion_vector_left,     ARGS_NONE());
  mrb_define_class_method(mrb, class_vector, "right",    mrb_leapmotion_vector_right,    ARGS_NONE());
  mrb_define_class_method(mrb, class_vector, "down",     mrb_leapmotion_vector_down,     ARGS_NONE());
  mrb_define_class_method(mrb, class_vector, "up",       mrb_leapmotion_vector_up,       ARGS_NONE());
  mrb_define_class_method(mrb, class_vector, "forward",  mrb_leapmotion_vector_forward,  ARGS_NONE());
  mrb_define_class_method(mrb, class_vector, "backward", mrb_leapmotion_vector_backward, ARGS_NONE());

  mrb_define_method(mrb, class_frame, "id",                      mrb_leapmotion_frame_id,                      ARGS_NONE());
  mrb_define_method(mrb, class_frame, "timestamp",               mrb_leapmotion_frame_timestamp,               ARGS_NONE());
  mrb_define_method(mrb, class_frame, "hands",                   mrb_leapmotion_frame_hands,                   ARGS_NONE());
  mrb_define_method(mrb, class_frame, "hand",                    mrb_leapmotion_frame_hand,                    ARGS_REQ(1));
  mrb_define_method(mrb, class_frame, "pointables",              mrb_leapmotion_frame_pointables,              ARGS_NONE());
  mrb_define_method(mrb, class_frame, "pointable",               mrb_leapmotion_frame_pointable,               ARGS_REQ(1));
  mrb_define_method(mrb, class_frame, "fingers",                 mrb_leapmotion_frame_fingers,                 ARGS_NONE());
  mrb_define_method(mrb, class_frame, "finger",                  mrb_leapmotion_frame_finger,                  ARGS_REQ(1));
  mrb_define_method(mrb, class_frame, "tools",                   mrb_leapmotion_frame_tools,                   ARGS_NONE());
  mrb_define_method(mrb, class_frame, "tool",                    mrb_leapmotion_frame_tool,                    ARGS_REQ(1));
  mrb_define_method(mrb, class_frame, "gestures",                mrb_leapmotion_frame_gestures,                ARGS_NONE());
  mrb_define_method(mrb, class_frame, "gesture",                 mrb_leapmotion_frame_gesture,                 ARGS_REQ(1));
  mrb_define_method(mrb, class_frame, "translation",             mrb_leapmotion_frame_translation,             ARGS_REQ(1));
  mrb_define_method(mrb, class_frame, "translation_probability", mrb_leapmotion_frame_translation_probability, ARGS_REQ(1));
  mrb_define_method(mrb, class_frame, "rotation_axis",           mrb_leapmotion_frame_rotation_axis,           ARGS_REQ(1));
  mrb_define_method(mrb, class_frame, "rotation_angle",          mrb_leapmotion_frame_rotation_angle,          ARGS_REQ(1));
  mrb_define_method(mrb, class_frame, "rotation_matrix",         mrb_leapmotion_frame_rotation_matrix,         ARGS_REQ(1));
  mrb_define_method(mrb, class_frame, "rotation_probability",    mrb_leapmotion_frame_rotation_probability,    ARGS_REQ(1));
  mrb_define_method(mrb, class_frame, "scale_factor",            mrb_leapmotion_frame_scale_factor,            ARGS_REQ(1));
  mrb_define_method(mrb, class_frame, "scale_probability",       mrb_leapmotion_frame_scale_probability,       ARGS_REQ(1));
  mrb_define_method(mrb, class_frame, "interaction_box",         mrb_leapmotion_frame_interaction_box,         ARGS_NONE());
  mrb_define_method(mrb, class_frame, "valid?",                  mrb_leapmotion_frame_is_valid,                ARGS_NONE());
  mrb_define_method(mrb, class_frame, "to_s",                    mrb_leapmotion_frame_to_s,                    ARGS_NONE());
  mrb_define_method(mrb, class_frame, "==",                      mrb_leapmotion_frame_equal,                   ARGS_REQ(1));

  mrb_define_class_method(mrb, class_frame, "invalid", mrb_leapmotion_frame_invalid, ARGS_NONE());

  mrb_define_method(mrb, class_handlist, "count" ,    mrb_leapmotion_handlist_count,     ARGS_NONE());
  mrb_define_method(mrb, class_handlist, "empty?",    mrb_leapmotion_handlist_is_empty,  ARGS_NONE());
  mrb_define_method(mrb, class_handlist, "[]",        mrb_leapmotion_handlist_get_at,    ARGS_REQ(1));
  mrb_define_method(mrb, class_handlist, "leftmost",  mrb_leapmotion_handlist_leftmost,  ARGS_NONE());
  mrb_define_method(mrb, class_handlist, "rightmost", mrb_leapmotion_handlist_rightmost, ARGS_NONE());
  mrb_define_method(mrb, class_handlist, "frontmost", mrb_leapmotion_handlist_frontmost, ARGS_NONE());

  mrb_define_method(mrb, class_hand, "id",                      mrb_leapmotion_hand_id,                      ARGS_NONE());
  mrb_define_method(mrb, class_hand, "frame",                   mrb_leapmotion_hand_frame,                   ARGS_NONE());
  mrb_define_method(mrb, class_hand, "pointables",              mrb_leapmotion_hand_pointables,              ARGS_NONE());
  mrb_define_method(mrb, class_hand, "pointable",               mrb_leapmotion_hand_pointable,               ARGS_REQ(1));
  mrb_define_method(mrb, class_hand, "fingers",                 mrb_leapmotion_hand_fingers,                 ARGS_NONE());
  mrb_define_method(mrb, class_hand, "finger",                  mrb_leapmotion_hand_finger,                  ARGS_REQ(1));
  mrb_define_method(mrb, class_hand, "tools",                   mrb_leapmotion_hand_tools,                   ARGS_NONE());
  mrb_define_method(mrb, class_hand, "tool",                    mrb_leapmotion_hand_tool,                    ARGS_REQ(1));
  mrb_define_method(mrb, class_hand, "palm_position",           mrb_leapmotion_hand_palm_position,           ARGS_NONE());
  mrb_define_method(mrb, class_hand, "palm_velocity",           mrb_leapmotion_hand_palm_velocity,           ARGS_NONE());
  mrb_define_method(mrb, class_hand, "palm_normal",             mrb_leapmotion_hand_palm_normal,             ARGS_NONE());
  mrb_define_method(mrb, class_hand, "direction",               mrb_leapmotion_hand_direction,               ARGS_NONE());
  mrb_define_method(mrb, class_hand, "sphere_center",           mrb_leapmotion_hand_sphere_center,           ARGS_NONE());
  mrb_define_method(mrb, class_hand, "sphere_radius",           mrb_leapmotion_hand_sphere_radius,           ARGS_NONE());
  mrb_define_method(mrb, class_hand, "translation",             mrb_leapmotion_hand_translation,             ARGS_REQ(1));
  mrb_define_method(mrb, class_hand, "translation_probability", mrb_leapmotion_hand_translation_probability, ARGS_REQ(1));
  mrb_define_method(mrb, class_hand, "rotation_axis",           mrb_leapmotion_hand_rotation_axis,           ARGS_REQ(1));
  mrb_define_method(mrb, class_hand, "rotation_angle",          mrb_leapmotion_hand_rotation_angle,          MRB_ARGS_ARG(1, 1));
  mrb_define_method(mrb, class_hand, "rotation_matrix",         mrb_leapmotion_hand_rotation_matrix,         ARGS_REQ(1));
  mrb_define_method(mrb, class_hand, "rotation_probability",    mrb_leapmotion_hand_rotation_probability,    ARGS_REQ(1));
  mrb_define_method(mrb, class_hand, "scale_factor",            mrb_leapmotion_hand_scale_factor,            ARGS_REQ(1));
  mrb_define_method(mrb, class_hand, "scale_probability",       mrb_leapmotion_hand_scale_probability,       ARGS_REQ(1));
  mrb_define_method(mrb, class_hand, "valid?",                  mrb_leapmotion_hand_is_valid,                ARGS_NONE());
  mrb_define_method(mrb, class_hand, "==",                      mrb_leapmotion_hand_equal,                   ARGS_NONE());
  mrb_define_method(mrb, class_hand, "to_s",                    mrb_leapmotion_hand_to_s,                    ARGS_NONE());

  mrb_define_method(mrb, class_pointablelist, "count",     mrb_leapmotion_pointablelist_count,     ARGS_NONE());
  mrb_define_method(mrb, class_pointablelist, "empty?",    mrb_leapmotion_pointablelist_is_empty,  ARGS_NONE());
  mrb_define_method(mrb, class_pointablelist, "[]",        mrb_leapmotion_pointablelist_get_at,    ARGS_REQ(1));
  mrb_define_method(mrb, class_pointablelist, "append",    mrb_leapmotion_pointablelist_append,    ARGS_REQ(1));
  mrb_define_method(mrb, class_pointablelist, "leftmost",  mrb_leapmotion_pointablelist_leftmost,  ARGS_NONE());
  mrb_define_method(mrb, class_pointablelist, "rightmost", mrb_leapmotion_pointablelist_rightmost, ARGS_NONE());
  mrb_define_method(mrb, class_pointablelist, "frontmost", mrb_leapmotion_pointablelist_frontmost, ARGS_NONE());

  mrb_define_method(mrb, class_pointable, "id",             mrb_leapmotion_pointable_id,             ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "frame",          mrb_leapmotion_pointable_frame,          ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "hand",           mrb_leapmotion_pointable_hand,           ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "tip_position",   mrb_leapmotion_pointable_tip_position,   ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "tip_velocity",   mrb_leapmotion_pointable_tip_velocity,   ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "directioni",     mrb_leapmotion_pointable_direction,      ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "width",          mrb_leapmotion_pointable_width,          ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "length",         mrb_leapmotion_pointable_length,         ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "finger?",        mrb_leapmotion_pointable_is_finger,      ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "tool?",          mrb_leapmotion_pointable_is_tool,        ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "valid?",         mrb_leapmotion_pointable_is_valid,       ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "touch_zone",     mrb_leapmotion_pointable_touch_zone,     ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "touch_distance", mrb_leapmotion_pointable_touch_distance, ARGS_NONE());
  mrb_define_method(
  	mrb, class_pointable, "stabilized_tip_position", mrb_leapmotion_pointable_stabilized_tip_position, ARGS_NONE());
  mrb_define_method(mrb, class_pointable, "==",             mrb_leapmotion_pointable_equal,          ARGS_REQ(1));
  mrb_define_method(mrb, class_pointable, "to_s",           mrb_leapmotion_pointable_to_s,           ARGS_NONE());

  mrb_define_method(mrb, class_fingerlist, "count",     mrb_leapmotion_fingerlist_count,     ARGS_NONE());
  mrb_define_method(mrb, class_fingerlist, "empty?",    mrb_leapmotion_fingerlist_is_empty,  ARGS_NONE());
  mrb_define_method(mrb, class_fingerlist, "[]",        mrb_leapmotion_fingerlist_get_at,    ARGS_REQ(1));
  mrb_define_method(mrb, class_fingerlist, "leftmost",  mrb_leapmotion_fingerlist_leftmost,  ARGS_NONE());
  mrb_define_method(mrb, class_fingerlist, "rightmost", mrb_leapmotion_fingerlist_rightmost, ARGS_NONE());
  mrb_define_method(mrb, class_fingerlist, "frontmost", mrb_leapmotion_fingerlist_frontmost, ARGS_NONE());

  mrb_define_method(mrb, class_finger, "to_s", mrb_leapmotion_finger_to_s, ARGS_NONE());

  mrb_define_class_method(mrb, class_finger, "invalid", mrb_leapmotion_finger_invalid, ARGS_NONE());

  mrb_define_method(mrb, class_toollist, "count",     mrb_leapmotion_toollist_count,     ARGS_NONE());
  mrb_define_method(mrb, class_toollist, "empty?",    mrb_leapmotion_toollist_is_empty,  ARGS_NONE());
  mrb_define_method(mrb, class_toollist, "[]",        mrb_leapmotion_toollist_get_at,    ARGS_REQ(1));
  mrb_define_method(mrb, class_toollist, "append",    mrb_leapmotion_toollist_append,    ARGS_REQ(1));
  mrb_define_method(mrb, class_toollist, "leftmost",  mrb_leapmotion_toollist_leftmost,  ARGS_NONE());
  mrb_define_method(mrb, class_toollist, "rightmost", mrb_leapmotion_toollist_rightmost, ARGS_NONE());
  mrb_define_method(mrb, class_toollist, "frontmost", mrb_leapmotion_toollist_frontmost, ARGS_NONE());

  mrb_define_method(mrb, class_tool, "to_s", mrb_leapmotion_tool_to_s, ARGS_NONE());

  mrb_define_class_method(mrb, class_tool, "invalid", mrb_leapmotion_tool_invalid, ARGS_NONE());

  mrb_define_method(mrb, class_gesturelist, "count",  mrb_leapmotion_gesturelist_count,    ARGS_NONE());
  mrb_define_method(mrb, class_gesturelist, "empty?", mrb_leapmotion_gesturelist_is_empty, ARGS_NONE());
  mrb_define_method(mrb, class_gesturelist, "[]",     mrb_leapmotion_gesturelist_get_at,   ARGS_REQ(1));
  mrb_define_method(mrb, class_gesturelist, "append", mrb_leapmotion_gesturelist_append,   ARGS_REQ(1));

  RClass *module_gesture_type = mrb_define_module_under(mrb, class_gesture, "Type");
  mrb_define_const(mrb, module_gesture_type, "TYPE_INVALID",    mrb_fixnum_value(Leap::Gesture::Type::TYPE_INVALID));
  mrb_define_const(mrb, module_gesture_type, "TYPE_SWIPE",      mrb_fixnum_value(Leap::Gesture::Type::TYPE_SWIPE));
  mrb_define_const(mrb, module_gesture_type, "TYPE_CIRCLE",     mrb_fixnum_value(Leap::Gesture::Type::TYPE_CIRCLE));
  mrb_define_const(mrb, module_gesture_type, "TYPE_SCREEN_TAP", mrb_fixnum_value(Leap::Gesture::Type::TYPE_SCREEN_TAP));
  mrb_define_const(mrb, module_gesture_type, "TYPE_KEY_TAP",    mrb_fixnum_value(Leap::Gesture::Type::TYPE_KEY_TAP));

  RClass *module_gesture_state = mrb_define_module_under(mrb, class_gesture, "State");
  mrb_define_const(mrb, module_gesture_state, "STATE_INVALID", mrb_fixnum_value(Leap::Gesture::State::STATE_INVALID));
  mrb_define_const(mrb, module_gesture_state, "STATE_START",   mrb_fixnum_value(Leap::Gesture::State::STATE_START));
  mrb_define_const(mrb, module_gesture_state, "STATE_UPDATE",  mrb_fixnum_value(Leap::Gesture::State::STATE_UPDATE));
  mrb_define_const(mrb, module_gesture_state, "STATE_STOP",    mrb_fixnum_value(Leap::Gesture::State::STATE_STOP));

  mrb_define_method(mrb, class_gesture, "type",            mrb_leapmotion_gesture_get_type,         ARGS_NONE());
  mrb_define_method(mrb, class_gesture, "state",           mrb_leapmotion_gesture_state,            ARGS_NONE());
  mrb_define_method(mrb, class_gesture, "id",              mrb_leapmotion_gesture_id,               ARGS_NONE());
  mrb_define_method(mrb, class_gesture, "duration",        mrb_leapmotion_gesture_duration,         ARGS_NONE());
  mrb_define_method(mrb, class_gesture, "durationSeconds", mrb_leapmotion_gesture_duration_seconds, ARGS_NONE());
  mrb_define_method(mrb, class_gesture, "frame",           mrb_leapmotion_gesture_frame,            ARGS_NONE());
  mrb_define_method(mrb, class_gesture, "hands",           mrb_leapmotion_gesture_hands,            ARGS_NONE());
  mrb_define_method(mrb, class_gesture, "valid?",          mrb_leapmotion_gesture_is_valid,         ARGS_NONE());
  mrb_define_method(mrb, class_gesture, "to_s",            mrb_leapmotion_gesture_to_s,             ARGS_NONE());

  mrb_define_method(mrb, class_swipegesture, "initialize",     mrb_leapmotion_swipegesture_initialize,     ARGS_REQ(1));
  mrb_define_method(mrb, class_swipegesture, "start_position", mrb_leapmotion_swipegesture_start_position, ARGS_NONE());
  mrb_define_method(mrb, class_swipegesture, "position",       mrb_leapmotion_swipegesture_position,       ARGS_NONE());
  mrb_define_method(mrb, class_swipegesture, "direction",      mrb_leapmotion_swipegesture_direction,      ARGS_NONE());
  mrb_define_method(mrb, class_swipegesture, "speed",          mrb_leapmotion_swipegesture_speed,          ARGS_NONE());
  mrb_define_method(mrb, class_swipegesture, "pointable",      mrb_leapmotion_swipegesture_pointable,      ARGS_NONE());

  mrb_define_method(mrb, class_circlegesture, "initialize", mrb_leapmotion_circlegesture_initialize, ARGS_REQ(1));
  mrb_define_method(mrb, class_circlegesture, "center",     mrb_leapmotion_circlegesture_center,     ARGS_NONE());
  mrb_define_method(mrb, class_circlegesture, "normal",     mrb_leapmotion_circlegesture_normal,     ARGS_NONE());
  mrb_define_method(mrb, class_circlegesture, "progress",   mrb_leapmotion_circlegesture_progress,   ARGS_NONE());
  mrb_define_method(mrb, class_circlegesture, "radius",     mrb_leapmotion_circlegesture_radius,     ARGS_NONE());
  mrb_define_method(mrb, class_circlegesture, "pointable",  mrb_leapmotion_circlegesture_pointable,  ARGS_NONE());

  mrb_define_method(mrb, class_screentapgesture, "initialize", mrb_leapmotion_screentapgesture_initialize, ARGS_REQ(1));
  mrb_define_method(mrb, class_screentapgesture, "position",   mrb_leapmotion_screentapgesture_position,   ARGS_NONE());
  mrb_define_method(mrb, class_screentapgesture, "direction",  mrb_leapmotion_screentapgesture_direction,  ARGS_NONE());
  mrb_define_method(mrb, class_screentapgesture, "progress",   mrb_leapmotion_screentapgesture_progress,   ARGS_NONE());
  mrb_define_method(mrb, class_screentapgesture, "pointable",  mrb_leapmotion_screentapgesture_pointable,  ARGS_NONE());

  mrb_define_method(mrb, class_keytapgesture, "initialize", mrb_leapmotion_keytapgesture_initialize, ARGS_REQ(1));
  mrb_define_method(mrb, class_keytapgesture, "position",   mrb_leapmotion_keytapgesture_position,   ARGS_NONE());
  mrb_define_method(mrb, class_keytapgesture, "direction",  mrb_leapmotion_keytapgesture_direction,  ARGS_NONE());
  mrb_define_method(mrb, class_keytapgesture, "progress",   mrb_leapmotion_keytapgesture_progress,   ARGS_NONE());
  mrb_define_method(mrb, class_keytapgesture, "pointable",  mrb_leapmotion_keytapgesture_pointable,  ARGS_NONE());

  /* for debug */
  mrb_define_method(mrb, mrb->kernel_module, "sleep", mrb_kernel_sleep, ARGS_REQ(1));

  mrb_leapmotion_type_map_t<Leap::Controller>::mrb_class       = class_controller;
  mrb_leapmotion_type_map_t<Leap::Config>::mrb_class           = class_config;
  mrb_leapmotion_type_map_t<Leap::Listener>::mrb_class         = class_listener;
  mrb_leapmotion_type_map_t<Leap::Vector>::mrb_class           = class_vector;
  mrb_leapmotion_type_map_t<Leap::Frame>::mrb_class            = class_frame;
  mrb_leapmotion_type_map_t<Leap::HandList>::mrb_class         = class_handlist;
  mrb_leapmotion_type_map_t<Leap::Hand>::mrb_class             = class_hand;
  mrb_leapmotion_type_map_t<Leap::PointableList>::mrb_class    = class_pointablelist;
  mrb_leapmotion_type_map_t<Leap::Pointable>::mrb_class        = class_pointable;
  mrb_leapmotion_type_map_t<Leap::ToolList>::mrb_class         = class_toollist;
  mrb_leapmotion_type_map_t<Leap::Tool>::mrb_class             = class_tool;
  mrb_leapmotion_type_map_t<Leap::FingerList>::mrb_class       = class_fingerlist;
  mrb_leapmotion_type_map_t<Leap::Finger>::mrb_class           = class_finger;
  mrb_leapmotion_type_map_t<Leap::GestureList>::mrb_class      = class_gesturelist;
  mrb_leapmotion_type_map_t<Leap::Gesture>::mrb_class          = class_gesture;
  mrb_leapmotion_type_map_t<Leap::SwipeGesture>::mrb_class     = class_swipegesture;
  mrb_leapmotion_type_map_t<Leap::CircleGesture>::mrb_class    = class_circlegesture;
  mrb_leapmotion_type_map_t<Leap::ScreenTapGesture>::mrb_class = class_screentapgesture;
  mrb_leapmotion_type_map_t<Leap::KeyTapGesture>::mrb_class    = class_keytapgesture;
}

void
mrb_mruby_leapmotion_gem_final(mrb_state *mrb)
{
}

}

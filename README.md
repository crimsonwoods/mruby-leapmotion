mruby-leapmotion
====

_mruby-leapmotion_ wrapped 'LeapSDK'.

# Supported platforms
----

- Ubuntu 12.04 LTS

# How to build
----

1. download and extract [LeapSDK](https://developer.leapmotion.com/downloads)
2. edit your 'build_config.rb'
3. run 'make' command.


build_config.rb:

    conf.gem :github => 'crimsonwoods/mruby-leapmotion', :branch => 'master'
    
    conf.cxx do |cxx|
      cxx.flags << "-std=c++11"
      cxx.include_paths << "/path/to/your/LeapSDK/include"
    end
    
    conf.linker do |linker|
      linker.libraries << %w(stdc++ m Leap)
      linker.library_paths << "/path/to/your/LeapSDK/lib/<platform>/"
    end


# Sample
----

[Sample code](/sample/leapmotion.rb) is contained into 'sample' directory.

# How to run
----

1. connect your Leap Motion
2. wait for chaning, the color of indicator will be green.
3. set LeapSDK library path to 'LD_LIBRARY_PATH'.
4. run mruby with sample code or your code.


# Module
----

- LeapMotion
- LeapMotion::Gesture::Type
- LeapMotion::Gesture::State

## LeapMotion::Gesture::Type

- TYPE_INVALID
- TYPE_SWIPE
- TYPE_CIRCLE
- TYPE_SCREEN_TAP
- TYPE_KEY_TAP

## LeapMotion::Gesture::State

- STATE_INVALID
- STATE_START
- STATE_UPDATE
- STATE_STOP

## LeapMotion::Controller::PolicyFlag

- POLICY_DEFAULT
- POLICY_BACKGROUND_FRAMES

## LeapMotion::Config::ValueType

- TYPE_UNKNOWN
- TYPE_BOOLEAN
- TYPE_INT32
- TYPE_FLOAT
- TYPE_STRING

# Class
----

- LeapMotion::Config
- LeapMotion::Controller
- LeapMotion::Listener
- LeapMotion::Vector
- LeapMotion::PointableList
- LeapMotion::Pointable
- LeapMotion::ToolList
- LeapMotion::Tool
- LeapMotion::HandList
- LeapMotion::Hand
- LeapMotion::FingerList
- LeapMotion::Finger
- LeapMotion::GestureList
- LeapMotion::Gesture
- LeapMotion::SwipeGesture
- LeapMotion::CircleGesture
- LeapMotion::ScreenTapGesture
- LeapMotion::KeyTapGesture

## LeapMotion::Controller

### Instance methods

|Name              |LeapSDK API      |Status         |
|:-----------------|:----------------|:-------------:|
|initialize        |ctor             |implemented    |
|policy_flags      |policyFlags      |implemented    |
|policy_flags=     |setPolicyFlags   |implemented    |
|add_listener      |addListener      |implemented    |
|remove_listener   |removeListener   |implemented    |
|frame             |frame            |implemented    |
|config            |config           |implemented    |
|devices           |devices          |not implemented|
|located_screens   |locatedScreens   |not implemented|
|calibrated_screens|calibratedScreens|not implemented|
|enable_gesture    |enableGesture    |implemented    |
|gesture_enabled?  |isGestureEnabled |implemented    |
|connected?        |isConnected      |implemented    |
|focus?            |hasFocus         |implemented    |

## LeapMotion::Listener

### Instance methods

|Name            |LeapSDK API      |Status         |
|:---------------|:----------------|:-------------:|
|on_init         |onInit           |implemented    |
|on_connect      |onConnect        |implemented    |
|on_disconnect   |onDisconnect     |implemented    |
|on_exit         |onExit           |implemented    |
|on_frame        |onFrame          |implemented    |
|on_focus_gained |onFocusGained    |implemented    |
|on_focus_lost   |onFocusLost      |implemented    |

## LeapMotion::Frame

### Instance methods

|Name                      |LeapSDK API                |Status         |
|:-------------------------|:--------------------------|:-------------:|
|id                        |id                         |implemented    |
|timestamp                 |timestamp                  |implemented    |
|hands                     |hands                      |implemented    |
|hand                      |hand                       |implemented    |
|pointables                |pointables                 |implemented    |
|pointable(id)             |pointable(id)              |implemented    |
|fingers                   |fingers                    |implemented    |
|finger(id)                |finger(id)                 |implemented    |
|tools                     |tools                      |implemented    |
|tool(id)                  |tool(id)                   |implemented    |
|gesture(id)               |gesture                    |implemented    |
|gestures                  |gestures()                 |implemented    |
|gestures(frame)           |gestures(frame)            |implemented    |
|translation               |translation                |implemented    |
|translation_probability   |translationProbability     |implemented    |
|rotation_axis             |rotationAxis               |implemented    |
|rotation_angle            |rotationAngle(frame)       |implemented    |
|rotation_angle(frame,axis)|rotationAngle(frame,vector)|implemented    |
|rotation_matrix           |rotationMatrix             |implemented    |
|rotation_probability      |rotationProbability        |implemented    |
|scale_factor              |scaleFactor                |implemented    |
|scale_probability         |scaleProbability           |implemented    |
|interaction_box           |interactionBox             |not implemented|
|valid?                    |isValid                    |implemented    |
|==                        |operator==                 |implemented    |
|(missing)                 |operator!=                 |not defined    |
|to_s                      |toString                   |implemented    |

# License

MIT License is applied.

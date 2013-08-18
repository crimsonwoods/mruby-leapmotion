mruby-leapmotion
====

_mruby-leapmotion_ wrapped 'LeapSDK'.

# Supported platforms
----

- Ubuntu 12.04 LTS
- Mac OS X
- Windows

# How to build
----

1. download and extract [LeapSDK](https://developer.leapmotion.com/downloads)
2. edit your 'build_config.rb'
3. set library path to load a library contained into LeapSDK (libLeap.so, libLeap.dylib).
4. run 'make' command.

## for Ubuntu

build_config.rb:

    conf.gem :github => 'crimsonwoods/mruby-leapmotion', :branch => 'master'
    
    conf.cxx do |cxx|
      cxx.include_paths << "/path/to/your/LeapSDK/include"
    end
    
    conf.linker do |linker|
      linker.libraries << %w(stdc++ m Leap)
      linker.library_paths << "/path/to/your/LeapSDK/lib/<platform>/"
    end

## for Mac

build_config.rb:

    toolchain :clang
    
    ...
    
    conf.gem :github => 'crimsonwoods/mruby-leapmotion', :branch => 'master'
    
    conf.cxx do |cxx|
      cxx.flags << "-stdlib=libc++"
      cxx.include_paths << "/path/to/your/LeapSDK/include"
    end
    
    conf.linker do |linker|
      linker.libraries << %w(c++ mLeap)
      linker.library_paths << "/path/to/your/LeapSDK/lib/libc++/"
    end

## for Windows

build_config.rb:

    toolchain :vs2012
    
    ...
    
    conf.gem :github => 'crimsonwoods/mruby-leapmotion', :branch => 'master'

    conf.cxx do |cxx|
      cxx.include_paths << "X:\\path\\to\\your\\LeapSDK\\include"
    end

    conf.linker do |linker|
      linker.libraries << %w(Leapd)
      linker.library_paths << "X:\\path\\to\\your\\LeapSDK\\lib\\<platform>"
    end

# Restrictions

## for Windows

1. Use 'Leapd.dll' (for debug) instead of 'Leap.dll'. Release version DLL is not working currently.

# Sample
----

[Sample code](/sample/leapmotion.rb) is contained into 'sample' directory.

# How to run
----

1. connect your Leap Motion
2. wait for chaning, the color of indicator will be green.
3. set LeapSDK library path to 'LD_LIBRARY_PATH' (use 'DYLD_LIBRARY_PATH' instead for Mac).
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

## LeapMotion::Pointable::Zone

- ZONE_NONE
- ZONE_HOVERING
- ZONE_TOUCHING


# Class
----

- LeapMotion::Config
- LeapMotion::Controller
- LeapMotion::DeviceList
- LeapMotion::Device
- LeapMotion::FingerList
- LeapMotion::GestureList
- LeapMotion::Gesture
    - LeapMotion::CircleGesture
    - LeapMotion::KeyTapGesture
    - LeapMotion::ScreenTapGesture
    - LeapMotion::SwipeGesture
- LeapMotion::HandList
- LeapMotion::Hand
- LeapMotion::InteractionBox
- LeapMotion::Listener
- LeapMotion::PointableList
- LeapMotion::Pointable
    - LeapMotion::Finger
    - LeapMotion::Tool
- LeapMotion::ScreenList
- LeapMotion::Screen
- LeapMotion::ToolList
- LeapMotion::Vector
- LeapMotion::Matrix

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
|devices           |devices          |implemented    |
|located_screens   |locatedScreens   |implemented    |
|calibrated_screens|calibratedScreens|implemented    |
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
|interaction_box           |interactionBox             |implemented    |
|valid?                    |isValid                    |implemented    |
|==                        |operator==                 |implemented    |
|(missing)                 |operator!=                 |not defined    |
|to_s                      |toString                   |implemented    |

## LeapMotion::HandList

## Instance methods

|Name       |LeapSDK API  |Status         |
|:----------|:------------|:-------------:|
|count      |count        |implemented    |
|empty?     |empty        |implemented    |
|[]         |operator[]   |implemented    |
|leftmost   |leftmost     |implemented    |
|rightmost  |rightmost    |implemented    |
|frontmost  |frontmost    |implemented    |
|each       |(missing)    |implemented    |
|(missing)  |begin        |not defined    |
|(missing)  |end          |not defined    |

## LeapMotion::Hand

### Instance methods

|Name                      |LeapSDK API                |Status         |
|:-------------------------|:--------------------------|:-------------:|
|id                        |id                         |implemented    |
|frame                     |frame                      |implemented    |
|pointables                |pointables                 |implemented    |
|pointable                 |pointable                  |implemented    |
|fingers                   |fingers                    |implemented    |
|finger                    |finger                     |implemented    |
|tools                     |tools                      |implemented    |
|tool                      |tool                       |implemented    |
|palm_position             |palmPosition               |implemented    |
|palm_velocity             |palmVelocity               |implemented    |
|palm_normal               |palmNormal                 |implemented    |
|direction                 |direction                  |implemented    |
|sphere_center             |sphereCenter               |implemented    |
|sphere_radius             |sphereRadius               |implemented    |
|translation               |translation                |implemented    |
|translation_probability   |translationProbability     |implemented    |
|rotation_axis             |rotationAxis               |implemented    |
|rotation_angle            |rotationAngle              |implemented    |
|rotation_matrix           |rotationMatrix             |implemented    |
|rotation_probability      |rotationProbability        |implemented    |
|scale_factor              |scaleFactor                |implemented    |
|scale_probability         |scaleProbability           |implemented    |
|valid?                    |isValid                    |implemented    |
|==                        |operator==                 |implemented    |
|!=                        |operator!=                 |not defined    |
|to_s                      |toString                   |implemented    |

# License

MIT License is applied.

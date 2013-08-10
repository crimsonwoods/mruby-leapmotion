mruby-leapmotion
====

_mruby-leapmotion_ wrapped 'LeapSDK'.


# How to build
----

edit your build_config.rb like this:

    conf.gem :github => 'crimsonwoods/mruby-leapmotion', :branch => 'master'
    
    conf.cxx do |cxx|
      cxx.flags << "-std=c++11"
      cxx.include_paths << "/path/to/your/LeapSDK/include"
    end
    
    conf.linker do |linker|
      linker.libraries << %w(stdc++ m Leap)
      linker.library_paths << "/path/to/your/LeapSDK/lib/<platform>/"
    end

and run 'make' command.


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

# Class
----

- LeapMotion::Controller
- LeapMotion::Listener
- LeapMotion::Vector
- LeapMotion::Pointable
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

|Name            |LeapSDK API      |Status         |
|:---------------|:----------------|:-------------:|
|add_listener    |addListener      |implemented    |
|remove_listener |removeListener   |implemented    |
|frame           |frame            |implemented    |
|config          |config           |not implemented|
|devices         |devices          |not implemented|
|connected?      |isConnected      |implemented    |
|focus?          |hasFocus         |implemented    |
|enable_gesture  |enableGesture    |implemented    |
|gesture_enabled?|isGestureEnabled |implemented    |
|(missing)       |policyFlags      |not defined    |
|(missing)       |setPolicyFlags   |not defined    |
|(missing)       |locatedScreens   |not defined    |
|(missing)       |calibratedScreens|not defined    |

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

|Name            |LeapSDK API                |Status         |
|:---------------|:--------------------------|:-------------:|
|id              |id                         |implemented    |
|timestamp       |timestamp                  |implemented    |
|hands           |hands                      |implemented    |
|hand            |hand                       |implemented    |
|(missing)       |pointables                 |not defined    |
|(missing)       |pointable                  |not defined    |
|(missing)       |fingers                    |not defined    |
|(missing)       |finger                     |not defined    |
|(missing)       |tools                      |not defined    |
|gesture         |gesture                    |implemented    |
|gestures        |gestures()                 |implemented    |
|gestures        |gestures(frame)            |not implemented|
|(missing)       |translation                |not defined    |
|(missing)       |translationProbability     |not defined    |
|(missing)       |rotationAxis               |not defined    |
|(missing)       |rotationAngle(frame)       |not defined    |
|(missing)       |rotationAngle(frame,vector)|not defined    |
|(missing)       |rotationMatrix             |not defined    |
|(missing)       |rotationProbability        |not defined    |
|(missing)       |scaleFactor                |not defined    |
|(missing)       |scaleProbability           |not defined    |
|(missing)       |interactionBox             |not defined    |
|valid?          |isValid                    |implemented    |
|(missing)       |operator==                 |not defined    |
|(missing)       |operator!=                 |not defined    |
|to_s            |toString                   |implemented    |



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

|Name            |Status         |
|:---------------|:-------------:|
|add_listener    |implemented    |
|remove_listener |implemented    |
|frame           |implemented    |
|config          |not implemented|
|devices         |not implemented|
|connected?      |implemented    |
|focus?          |implemented    |
|enable_gesture  |implemented    |
|gesture_enabled?|implemented    |


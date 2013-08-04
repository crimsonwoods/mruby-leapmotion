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


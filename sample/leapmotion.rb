class MyListener < LeapMotion::Listener
  def on_init(controller)
    puts "on_init"
  end
  def on_connect(controller)
    puts "on_connect"
	controller.enable_gesture(LeapMotion::Gesture::Type::TYPE_SWIPE)
	controller.enable_gesture(LeapMotion::Gesture::Type::TYPE_CIRCLE)
	controller.enable_gesture(LeapMotion::Gesture::Type::TYPE_SCREEN_TAP)
	controller.enable_gesture(LeapMotion::Gesture::Type::TYPE_KEY_TAP)
  end
  def on_disconnect(controller)
    puts "on_disconnect"
  end
  def on_exit(controller)
    puts "on_exit"
  end
  def on_frame(controller)
    frame = controller.frame
    puts "on_frame: " + frame.to_s
    hands = frame.hands
    puts "  hands: " + hands.count.to_s
    i = 0;
    while i < hands.count
      puts "    hand: " + hands[i].to_s
      fingers = hands[i].fingers
      j = 0
      while j < fingers.count
        puts "      finger: " + fingers[j].tip_position.to_s
        j += 1
      end
      i += 1
    end
    gestures = frame.gestures
    puts "  gestures: " + gestures.count.to_s
    i = 0;
    while i < gestures.count
      type = case gestures[i].type
      when LeapMotion::Gesture::Type::TYPE_SWIPE then
        swipe = LeapMotion::SwipeGesture.new(gestures[i])
        puts "    gesture: SWIPE start=" + swipe.start_position.to_s
      when LeapMotion::Gesture::Type::TYPE_CIRCLE then
        circle = LeapMotion::CircleGesture.new(gestures[i])
        puts "    gesture: CIRCLE radius=" + circle.radius.to_s
      when LeapMotion::Gesture::Type::TYPE_SCREEN_TAP then
        screen_tap = LeapMotion::ScreenTapGesture.new(gestures[i])
        puts "    gesture: SCREEN_TAP position=" + screen_tap.position.to_s
      when LeapMotion::Gesture::Type::TYPE_KEY_TAP then
        key_tap = LeapMotion::KeyTapGesture.new(gestures[i])
        puts "    gesture: KEY_TAP position=" + key_tap.position.to_s
      end
      i += 1;
    end
  end
  def on_focus_gained(controller)
    puts "on_focus_gained"
  end
  def on_focus_lost(controller)
    puts "on_focus_lost"
  end
end

controller = LeapMotion::Controller.new
listener = MyListener.new
controller.add_listener(listener)

puts "start LeapMotion sample code."

sleep 10

controller.remove_listener(listener)


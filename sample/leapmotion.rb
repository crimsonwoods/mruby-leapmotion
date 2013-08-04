class MyListener < LeapMotion::Listener
  def on_init(controller)
    puts "on_init"
  end
  def on_connect(controller)
    puts "on_connect"
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
        j = j + 1
      end
      i = i + 1
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


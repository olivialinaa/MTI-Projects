# Challenge

“Chindōgu means unusual/rare/precious (chin) tool (dōgu). It originated from Japan, by an inventor by the name of Kenji Kawakami. Sometimes it is known by another term, while recognised as a different name in other parts of the world: unuseless inventions, weird tools, kludge, jugaad, Système D.
Despite the rather humorous approach to innovation, the practice of chindōgu can be seen as an exercise in creativity, unhindered by the penalty of impracticality. This impracticality might be physiological, ethical, cultural, social, oftentimes just simply too embarrassing to use. However, one can argue that chindōgu embraces a kind of hopeful, even childish optimism that any problem can be resolved, and the mistakes we make in such quick attempts are hopefully worthy lessons.
Given the pandemic across the world, borders have been closed for so long. Now that frontiers are again open, we are all itching to get back out into the world, near and far. This brief requires you to address travel, or (alternatively) lack thereof. Take into consideration the logistics, fears, needs of the traveller and use a “Chindogu” approach to create a whimsical ‘travel’ device.
Design an Arduino based based solution that adopts the philiosophy of Chindōgu. Come up with an interactive contraption that supports, resolves, reflects or critiques on the various challenges associated with travel. Be as absurd and poetic, even nonsensical, as you want it to be, or if you wish, focus on the functional.”

# Design

For my travel Chindogu project, I got inspired by an experience I also have myself sometimes: When traveling we often wants to capture everything on camera and this is so easy nowadays when we can easily take pictures of everything with our smartphones. But then we often end up just experiencing the world through our phone instead of actually enjoying the moment. We just look at our phone searching for the way, looking through social media or are hunting for the perfect picture and easily forget the world around us. Besides, it’s not only about missing out
on making memories, but walking around with your phone all the time is also a safety issue as well. Murphy (2007) found that “talking on a mobile phone is associated with cognitive distraction that may undermine pedestrian safety”. 

Based on that, I decided to focus my design on reminding one to experience the world
around us while traveling without constantly looking at your phone. Therefore, I decided to make a design that turns your glasses black whenever you look at your phone as a gentle reminder that says:

#### Either your phone is experiencing the world around you, or you are experiencing it yourself!

So you can still take pictures, but won’t be able to see the pictures you actually take. Or you can experience your surroundings with your own eyes, but won’t be able to capture them with your phone at the same time. So, it’s a useless and annoying product, that makes you look ridiculous every time you take out your phone and it won’t allow you to take nice pictures or look up your way on Google Maps, but at the same time it’s useless (unuseless in Chindogu) because it makes you pay full attention to your surroundings and enjoy the moment while traveling.

# Code

The arduino code can be found [here!](Assignment2.ino)

# Circuit Diagram & Wiring

### Component Selection

### INPUT

To prototype the chosen design, input was needed that would resemble using a phone. Therefore,
an RFID sensor was chosen to detect if someone is holding the phone by detecting when the RFID tag is close to the sensor. To prevent wrong detection, e.g. while someone is holding the phone, but inside a pocket, a photoresistor is chosen that measures if it is dark (inside the pocket) or bright (outside the pocket). In addition, a toggle switch was added to resemble turning
on and off the phone screen, to prevent the mechanism from being activated when someone is just
holding the phone but not actually using it.

## OUTPUT

To make the glasses turn black when the phone is being used, a servo motor was chosen to enable
the movement of the cover. To also have a warning light and sound, a LED and piezo buzzer were added as outputs.

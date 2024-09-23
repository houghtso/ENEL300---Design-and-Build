# Equipment
- Arduino (UNO or ENEL198 produced)
- Connector cable
- Circuit and housing unit ( or just the circuit and LCD screen!)
- Small flathead screwdriver or potentiometer screwdriver

# Game Setup
- Connect the circuit to the arduino (header pins are the same!) and place back in housing.
- Connect the arduino to a computer and download the file (from this repository) called "300FinalCODE.ino"
- Open up the Arduino IDE and then open the file (or open the file and it'll do it automatically!)
- Upload the code to the Arduino
- Yay, you have started up the game!

# Gameplay
The game is styled on Flappy Bird (2013), you need to decide which filter you would like to run by sliding the switch in the ciruit,
then adjust the potentiometer to the left or right to increase or decrease the height of the line, don't hit the block at the end!

# Scoring
The first person to 3/3 wins!

# Questions
 - Connect the test points of each filter to an oscilloscope and vary the resistance. See if you can identify from the waveforms which is the high-pass filter, and the low-pass filter. What features helped you with identifying this?
 - The input signal is a square wave, is there any impact on high-pass or low-pass filters due to this? What are those circuits called?
 - What happens to the amplitude of the waveform as you increase, or decrease the resistance, and does this change your high-pass, low-pass or other, decision?
 - Can you describe a real world scenario where high-pass, and low-pass filters are used?
 - Can you describe a real world scenario where RC Differentiators, and Integrators are used?

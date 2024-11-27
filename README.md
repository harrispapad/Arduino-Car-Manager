# Arduino-Car-Manager
Arduino-based code to manage the windshield wipers and lighting system of a project electric car, enabling control for enhanced functionality with very low memory usage.

## How It Works  
1. **Windshield Wipers**:  
   - Stepper motor rotates clockwise or counterclockwise to simulate wiper motion.  
   - Speed and direction are defined in the setup.  

2. **Lighting System**:  
   - Buttons control alarm and indicator states via pin-change interrupts.  
   - Timer interrupts handle flashing logic for alarm and indicators.  

3. **Debounce Handling**:  
   - Ensures accidental button presses are ignored with a delay of 100ms.  

4. **Debugging**:  
   - Serial monitor prints status messages for easy debugging and monitoring.

  ## Pin Configuration  
| Component             | Arduino Pin |  
|-----------------------|-------------|  
| Stepper Motor         | 5, 6, 9, 10 |  
| Alarm Button          | 2           |  
| Right Flash Button    | 8           |  
| Left Flash Button     | 14 (A0)     |  
| Right Flash LEDs      | 3, 4        |  
| Left Flash LEDs       | 5, 6        |  
| Wiper Enable (ENA)    | 13          |

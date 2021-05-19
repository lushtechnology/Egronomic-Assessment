# Egronomic-Assessment
This project is a Bluetooth Low Energy (BLE) application that collects data from a peripheral device ( Adafruit feather sense) over BLE, and feeds it to a machine learning classifier
for classifying ergonomic vs non-ergonomic human body postures. In detail the app do the following :

- Collect data from bluetooth stack and store it in a buffer.
- A neural network based classifer that was trained on manually labeled dataset using Edge Impulse Studio can classify between ergonomics and non ergonomic position for workers (lower back position).
- Sending notification to the mobile nrf connect app whenever non-egronomic position was detected. 

# Hardware requirements
- Peripheral device that sends orientation data (euler angles) over BLE (we used Adafruit feather sense).
- Nordic semiconductor nrf5340 developement kit.

# Sofware requirements
- Nordic semiconductor SDK.
- NRF connect mobile app. 

# Setup

- For setup, the app should be built and deployed to nrf5340dk using segger embdedded studio.

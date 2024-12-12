int barcodeRead() {
  volatile long int white_start_time = millis();

  nilakna.drive(FORWARD);
  // pid constants for encoder
  Serial.println("Start");

  int result;

  while (1) {
    equalizeEncoderCounts();
    byte onesandzeros[numIRSensors];
    pravindu.getDigitalReadingsArray(onesandzeros);

    white_detected = false;

    // Update white_detected if the middle of the array detects white
    if ((1 - digitalRead(leftmostIR) == 1) && (1 - digitalRead(rightmostIR) == 1)) {
      white_detected = true;
    }

    if (white_detected && white_start_time == 0) {
      // Start of a white line
      white_start_time = micros();
      encoder_count_start = leftWheel.encoderCount;  // Capture encoder count at the start of the white line
    }

    encoder_count_end = leftWheel.encoderCount;  // Capture encoder count at the end of the white line

    if (!white_detected && white_start_time != 0) {  //(encoder_count_end - encoder_count_start > 50)
      // End of a white line
      encoder_count_end = leftWheel.encoderCount;  // Capture encoder count at the end of the white line
      count_difference = encoder_count_end - encoder_count_start;
      Serial.println(count_difference);

      if (count_difference > THRESHOLD_COUNT) {
        resultArray[resultArrayIndex] = 1;
        Serial.println("Binary 1");
        analogWrite(A2, 155);
      } else {
        resultArray[resultArrayIndex] = 0;
        Serial.println("Binary 0");
        analogWrite(A2, 0);
      }
      // Reset timing and array index for next detection
      white_start_time = 0;
      resultArrayIndex++;
    }

    if (resultArrayIndex > 4 && resultArray[resultArrayIndex - 1] == 0 && resultArray[resultArrayIndex - 2] == 0 && resultArray[resultArrayIndex - 3] == 0) {
      // Stop Condition
      Serial.println("Stopping");
      nilakna.brake();

      for (int i = 1; i <= 8; i++) {
        binaryNumber = binaryNumber + resultArray[9 - i] * pow(2, i - 1);
        // Serial.println(resultArray[i] * pow(2, i-1));
      }
      resultArrayIndex++;  // To prevent further looping within the while(1). do not remove without counter measures
      result = int(round(binaryNumber) % 5);
      Serial.println(round(binaryNumber));
      Serial.println(result);

      if (round(binaryNumber) == 147) {
        analogWrite(A2, 155);
        delay(1000);
      }
      
      break;
    }
  }

  return result;
}

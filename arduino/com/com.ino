String input;
String identifier;
String remaining;

String x;
String y;
String z;

String last;
String trailing;

void setup() {
 Serial.begin(2000000);
 Serial.setTimeout(1);
}

void open() {
  Serial.println("-------------OPEN-------------");
}

void close() {
  Serial.println("------------CLOSED------------");
}

void right() {
  Serial.println("RIGHT");
}

void left() {
  Serial.println("LEFT");
}

void position(String x, String y, String z) {
  Serial.println("XYZ: " + x + "," + y + "," + z);
}

void loop() {
 while (!Serial.available());
 input = Serial.readString();
 identifier = input.substring(0,3);
 remaining = input.substring(3);
 
 char Buf[50];
 remaining.toCharArray(Buf, 50);
 
 if(identifier == "xyz") {
  x = String(strtok(Buf, ","));
  y = String(strtok(NULL, ","));

  last = String(strtok(NULL, ","));
  z = last.substring(0,3);
  trailing = last.substring(3);

  position(x,y,z);

  if(trailing == "ope") {
    open();
  }

  if(trailing == "clo") {
    close();
  }

  if(trailing == "rig") {
    right();
  }

  if(trailing == "lef") {
    left();
  }
 }
 
}

#ifndef YM7128_DIGITAL_DELAY
#define YM7128_DIGITAL_DELAY

// For a schematic of the YM7182 please see:
// http://www.dtech.lv/files_ym/ym7128.pdf

class YMDelayDevice {
  public:
    
    enum Attenuation {
      dbOff = 0,
      dbInfinity = 0,
      db60,
      db58, db56, db54, db52, db50,
      db48, db46, db44, db42, db40,
      db38, db36, db34, db32, db30,
      db28, db26, db24, db22, db20,
      db18, db16, db14, db12, db10,
      db8, db6, db4, db2, db0
    };

    enum Delay {
      by0 = 0,
      by0000, by0032, by0065, by0097, by0129, by0161, by0193, by0226,
      by0258, by0290, by0323, by0355, by0387, by0419, by0452, by0484, 
      by0516, by0549, by0581, by0613, by0645, by0678, by0710, by0742, 
      by0774, by0807, by0839, by0871, by0904, by0936, by0968, by1000
    };

 
    YMDelayDevice(int pinSCI, int pinA0, int pinDIN) {
      sci = pinSCI;
      a0 = pinA0;
      din = pinDIN;
 
      pinMode(sci,OUTPUT);
      pinMode(a0,OUTPUT);
      pinMode(din,OUTPUT);

      reset();
    }
    
    void reset() {
      setAttenuationVM(db0);
      setAttenuationVC(db0);
      setAttenuationVL(db0);
      setAttenuationVR(db0);
      
      setLowPassFilter(0, 0);
      
      for(int i=0;i<=8;++i) {
        setTapDelay(i, by0);
        setAttenuationTapLeft(i, dbInfinity);
        setAttenuationTapRight(i, dbInfinity);
      }
    }
    
    YMDelayDevice::Delay getDelayAt(const int index) {
       YMDelayDevice::Delay mapper[] = {
         by0000, by0032, by0065, by0097, by0129, by0161, by0193, by0226,
         by0258, by0290, by0323, by0355, by0387, by0419, by0452, by0484, 
         by0516, by0549, by0581, by0613, by0645, by0678, by0710, by0742, 
         by0774, by0807, by0839, by0871, by0904, by0936, by0968, by1000
       };
       
       return mapper[index];
    }
    
    YMDelayDevice::Attenuation getAttenuationAt(const int index) {
       YMDelayDevice::Attenuation mapper[] = {
          dbInfinity,
          db60,
          db58, db56, db54, db52, db50,
          db48, db46, db44, db42, db40,
          db38, db36, db34, db32, db30,
          db28, db26, db24, db22, db20,
          db18, db16, db14, db12, db10,
          db8, db6, db4, db2, db0
       };
       
       return mapper[index];
    }
    
    void setTapDelay(int tap, Delay param) {
      sendData(tap + T0, (int)param);
    }
    

    /*
    tap: 1 to 8, since tap 0 can't be set
    */
    void setAttenuationTapLeft(int tap, Attenuation value, const boolean inPhase = true) {
      if (tap) {
        sendData(tap + GL1 - 1, (int)value + (inPhase ? 32 : 0));
      }
    }
    
    void setAttenuationTapRight(int tap, Attenuation value, const boolean inPhase = true) {
      if (tap) {
        sendData(tap + GR1 - 1, (int)value);
      }
    }
    
    void setAttenuationVM(Attenuation value) {
      sendData(VM, (int)value);
    }
    
    void setAttenuationVC(Attenuation value) {
      sendData(VC, (int)value);
    }
    
    void setAttenuationVL(Attenuation value) {
      sendData(VL, (int)value);
    }
    
    void setAttenuationVR(Attenuation value) {
      sendData(VR, (int)value);
    }
    
    void setLowPassFilter(int c0, int c1) {
      sendData(C0, c0);
      sendData(C1, c1);
    }
    
    
private:
  int sci, a0, din;
  
  enum Address {
    GL1 = 0,  /* Left channel tap. Data: Attenuation value. bit 5: sign */
    GL2,
    GL3,
    GL4,
    GL5,
    GL6,
    GL7,
    GL8,
    GR1,      /* Right channel tap. Data: Attenuation value. bit 5: sign */
    GR2,
    GR3,
    GR4,
    GR5,
    GR6,
    GR7,
    GR8,
    VM = 0x10,  /* Attenuation value. bit 5: sign */
    VC,
    VL,
    VR,
    C0 = 0x14,  /* FIR coefficient */
    C1,
    T0,        /* Tap position */
    T1,
    T2,
    T3,
    T4,
    T5,
    T6,
    T7,
    T8
  };
  
  void sendData(int address, int data) {
    
    int dlay = 20;  // paranoidly slow response, to let the YM7128 respond. should only be 200 ns
    
    // prepare
    digitalWrite(sci, HIGH);  
    digitalWrite(a0, LOW);  
    
    // address
    for(int i=7;i>=0;--i) {
      digitalWrite(din, (1<<i) & address);  
      digitalWrite(sci, LOW);
      delayMicroseconds(dlay);
      digitalWrite(sci, HIGH);
    }
    
    // Switch to data
    digitalWrite(a0, HIGH);  
    delayMicroseconds(dlay);
      
    // data
    for(int i=7;i>=0;--i) {
      digitalWrite(din, (1<<i) & data);  
      digitalWrite(sci, LOW);
      delayMicroseconds(dlay);
      digitalWrite(sci, HIGH);
    }
    
    digitalWrite(a0, LOW);  
      
  }
  
};

#endif


#include <armstrong.h>
#include <ym7128.h>


YMDelayDevice *pDelayUnit;
int gCurrentPreset = 0;

const int pinYMsci = 5;
const int pinYMa0  = 6;
const int pinYMdin = 7;

const int pinButton = 2;
const int chlInputButton = 0;


class YMDelayPreset {
  public:
    void static setPreset(YMDelayDevice *pDevice, int idx) {
      pDevice->reset();
      
      switch(idx) {
        case 1:
          setPresetBasic(pDevice);
          break;
        case 2:
          setPresetMultiple(pDevice);
          break;
        case 3:
          setPresetHeavy(pDevice);
          break;
        default:
          setPresetOff(pDevice);
          break;
      }
    }
    
    void static setPresetOff(YMDelayDevice *pDevice) {
      // Assuming a falling edge on sci latches the DIN
      // Assume reset is already called
    }
    
    void static setPresetBasic(YMDelayDevice *pDevice) {
      pDevice->setTapDelay(1, YMDelayDevice::by0032);
      pDevice->setAttenuationTapLeft(1, YMDelayDevice::db0);
      pDevice->setAttenuationTapRight(1, YMDelayDevice::db2);
    }
    
    void static setPresetMultiple(YMDelayDevice *pDevice) {
      pDevice->setTapDelay(1, YMDelayDevice::by0032);
      pDevice->setAttenuationTapLeft(1, YMDelayDevice::db0);
      pDevice->setAttenuationTapRight(1, YMDelayDevice::db2);
      
      pDevice->setTapDelay(2, YMDelayDevice::by0290);
      pDevice->setAttenuationTapLeft(2, YMDelayDevice::db0);
      pDevice->setAttenuationTapRight(2, YMDelayDevice::db0);
      
      pDevice->setTapDelay(3, YMDelayDevice::by0549);
      pDevice->setAttenuationTapLeft(3, YMDelayDevice::db0);
      pDevice->setAttenuationTapRight(3, YMDelayDevice::db0);
    }
    
    void static setPresetHeavy(YMDelayDevice *pDevice) {
      pDevice->setTapDelay(1, YMDelayDevice::by0000);
      pDevice->setAttenuationTapLeft(1, YMDelayDevice::db0);
      pDevice->setAttenuationTapRight(1, YMDelayDevice::db0);

      pDevice->setTapDelay(2, YMDelayDevice::by1000);
      pDevice->setAttenuationTapLeft(2, YMDelayDevice::db2);
      pDevice->setAttenuationTapRight(2, YMDelayDevice::db2);
    }
};

void setup(){

  pDelayUnit = new YMDelayDevice(pinYMsci, pinYMa0, pinYMdin);
  YMDelayPreset::setPreset(pDelayUnit, gCurrentPreset);
  
  adiInitializePushButton(chlInputButton, pinButton);
}

void loop(){
  adiUpdate();
  
  if (adiIsPressed(chlInputButton)) {
    gCurrentPreset = (gCurrentPreset + 1) & 3;
    YMDelayPreset::setPreset(pDelayUnit, gCurrentPreset);
  }
}

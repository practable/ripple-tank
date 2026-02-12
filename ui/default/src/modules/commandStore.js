//Store for sending commands through the dataSocket


const commandStore = {
    state: () => ({
        dataSocket: null,
        currentMode: 'stop',     //'start', 'pulse'
        currentState: 'STATE_WAIT',      //reported state from hardware, do I need this in the UI?
        driver: {
            'hz': 24, 
            'from_hardware_hz': 24, 
            'max':99, 
            'min':1, 
            'step':1,       //for the input slider only, buttons can make the step smaller
            'pulse': 100,    //pulse time in ms, only applied in pulse mode
            'pulse_min': 1,
            'pulse_max': 999,   //ms
            'pulse_step': 1,    //ms, for slider input only
            'amplitude': 20,   //percentage, was previously absolute value between 0 - 255 for hardware
            'from_hardware_amplitude': 0,
            'amplitude_step': 1,    //for input slider only
            'amplitude_min': 0,
            'amplitude_max': 50        //percentage, was previously 0-255
        },
        lamp: {
            'brightness': 50,          //percentage, previously 0-1024
            'from_hardware_brightness': 0,
            'brightness-step': 1,
            'brightness_min': 0,
            'brightness_max': 100       //previously was up to 1024 but has been changed to a percentage value   
        }
       }),
       mutations:{
        SET_DATA_SOCKET(state, socket){
            state.dataSocket = socket;
        },
        SET_STOP(state){
            console.log('STOP MODE SET');
            state.currentMode = 'stop';
        },
        SET_START(state){
            console.log('START MODE SET');
            state.currentMode = 'start';
        },
        SET_PULSE(state){
             console.log('PULSE MODE SET');
            state.currentMode = 'pulse';
        },
        COMMAND_STOP(state){
            console.log('STOP COMMAND SENT');
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "stop"
                }));
            }
        },
        COMMAND_START(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "start"
                }));
            }
        },
        COMMAND_PULSE(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "pulse",
                    to: state.driver.pulse
                }));
            }
        },
        UPDATE_DRIVING_FREQUENCY(state, val){
            state.driver.hz = Number(val);
        },
        UPDATE_PULSE_LENGTH(state, val){
            state.driver.pulse = Number(val);
        },
        UPDATE_AMPLITUDE(state, val){
            state.driver.amplitude = Number(val);
        },
        UPDATE_BRIGHTNESS(state, val){
            state.lamp.brightness = Number(val);
        },
        COMMAND_UPDATE_DRIVING_FREQUENCY(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "hz",
                    to: state.driver.hz
                }));
            }
        },
        COMMAND_UPDATE_AMPLITUDE(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "amp",
                    to: state.driver.amplitude
                }));
            }
        },
        COMMAND_UPDATE_BRIGHTNESS(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "light",
                    to: state.lamp.brightness
                }));
            }
        },
        COMMAND_PUMP_OUT(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "unpump"
                }));
            }
        },
        COMMAND_PUMP_IN(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "pump"
                }));
            }
        },
        COMMAND_PUMP_STOP(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "stpump"
                }));
            }
        },
        SET_REPORTED_DRIVING_FREQUENCY(state, val){
            state.driver['from_hardware_hz'] = val;
        },
        SET_REPORTED_AMPLITUDE(state, val){
            state.driver['from_hardware_amplitude'] = val;
        },
        SET_REPORTED_BRIGHTNESS(state, val){
            state.lamp['from_hardware_brightness'] = val;
        },
        SET_MAX_FREQUENCY(state, max){
            state.driver.max = max;
        },
        SET_MAX_AMPLITUDE(state, max){
            state.driver["amplitude_max"] = max;
        },
        SET_MAX_BRIGHTNESS(state, max){
            state.lamp["brightness_max"] = max;
        },

       },
       actions:{
        setDataSocket(context, socket){
            context.commit("SET_DATA_SOCKET", socket);
        },
        setModeStop(context){
            context.commit('SET_STOP');
        },
        setModeStart(context){
            context.commit('SET_START');
        },
        setModePulse(context){
            context.commit('SET_PULSE');
        },
        sendCommandStop(context){
            context.commit('COMMAND_STOP');
        },
        sendCommandStart(context){
            context.commit('COMMAND_START');
        },
        sendCommandPulse(context){
            context.commit('COMMAND_PULSE');
        },
        updateDrivingFrequency(context, val){
            let to_set = val;
            if(val < context.getters.getDrivingFrequencyMin){
                to_set = context.getters.getDrivingFrequencyMin;
            } else if(val > context.getters.getDrivingFrequencyMax){
                to_set = context.getters.getDrivingFrequencyMax;
            }
            context.commit('UPDATE_DRIVING_FREQUENCY', to_set);
        },
        updatePulseLength(context, val){
            let to_set = val;
            if(val < context.getters.getPulseLengthMin){
                to_set = context.getters.getPulseLengthMin;
            } else if(val > context.getters.getPulseLengthMax){
                to_set = context.getters.getPulseLengthMax;
            }
            context.commit('UPDATE_PULSE_LENGTH', to_set);
        },
        updateAmplitude(context, val){
            let to_set = val;
            if(val < context.getters.getAmplitudeMin){
                to_set = context.getters.getAmplitudeMin;
            } else if(val > context.getters.getAmplitudeMax){
                to_set = context.getters.getAmplitudeMax;
            }
            context.commit('UPDATE_AMPLITUDE', to_set);
        },
        updateBrightness(context, val){
            let to_set = val;
            if(val < context.getters.getBrightnessMin){
                to_set = context.getters.getBrightnessMin;
            } else if(val > context.getters.getBrightnessMax){
                to_set = context.getters.getBrightnessMax;
            }
            context.commit('UPDATE_BRIGHTNESS', to_set);
        },
        sendCommandUpdateDrivingFrequency(context){
            context.commit('COMMAND_UPDATE_DRIVING_FREQUENCY');
           // context.commit('SET_REPORTED_DRIVING_FREQUENCY', context.getters.getDrivingFrequency)   //TEMP for setting a value from hardware
        },
        sendCommandUpdateAmplitude(context){
            context.commit('COMMAND_UPDATE_AMPLITUDE');
            //context.commit('SET_REPORTED_AMPLITUDE', context.getters.getAmplitude)   //TEMP for setting a value from hardware
        },
        sendCommandUpdateBrightness(context){
            context.commit('COMMAND_UPDATE_BRIGHTNESS');
            //context.commit('SET_REPORTED_BRIGHTNESS', context.getters.getBrightness)   //TEMP for setting a value from hardware
        },
        setReportedDrivingFrequency(context, val){
            context.commit('SET_REPORTED_DRIVING_FREQUENCY', val);
        },
        setReportedAmplitude(context, val){
            context.commit('SET_REPORTED_AMPLITUDE', val);
        },
        setReportedBrightness(context, val){
            context.commit('SET_REPORTED_BRIGHTNESS', val);
        },
        sendCommandPumpOut(context){
            context.commit('COMMAND_PUMP_OUT');
        },
        sendCommandPumpIn(context){
            context.commit('COMMAND_PUMP_IN');
        },
        sendCommandPumpStop(context){
            context.commit('COMMAND_PUMP_STOP');
        },
        setMaxFrequency(context, max){
            context.commit('SET_MAX_FREQUENCY', max);
        },
        setMaxAmplitude(context, max){
            context.commit('SET_MAX_AMPLITUDE', max);
        },
        setMaxBrightness(context, max){
            context.commit('SET_MAX_BRIGHTNESS', max);
        }
       },
       getters:{
        getDataSocket(state){
            return state.dataSocket;
        },
        getCurrentMode(state){
            return state.currentMode;
        },
        getDrivingFrequencyMin(state){
            return state.driver.min;
        },
        getDrivingFrequencyMax(state){
            return state.driver.max;
        },
        getDrivingFrequencyStep(state){
            return state.driver.step;
        },
        getDrivingFrequency(state){
            return state.driver.hz;
        },
        getReportedDrivingFrequency(state){
            return state.driver['from_hardware_hz'];
        },
        getPulseLength(state){
            return state.driver.pulse;
        },
        getPulseLengthMin(state){
            return state.driver['pulse_min'];
        },
        getPulseLengthMax(state){
            return state.driver['pulse_max'];
        },
        getPulseLengthStep(state){
            return state.driver['pulse_step'];
        },
        getAmplitudeMin(state){
            return state.driver['amplitude_min'];
        },
        getAmplitudeMax(state){
            return state.driver['amplitude_max'];
        },
        getAmplitudeStep(state){
            return state.driver['amplitude_step'];
        },
        getAmplitude(state){
            return state.driver.amplitude;
        },
        getReportedAmplitude(state){
            return state.driver['from_hardware_amplitude'];
        },
        getBrightnessMin(state){
            return state.lamp['brightness_min'];
        },
        getBrightnessMax(state){
            return state.lamp['brightness_max'];
        },
        getBrightnessStep(state){
            return state.lamp['brightness_step'];
        },
        getBrightness(state){
            return state.lamp.brightness;
        },
        getReportedBrightness(state){
            return state.lamp['from_hardware_brightness'];
        },
          
       },  
  
  }

  export default commandStore;
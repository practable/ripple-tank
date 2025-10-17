//Vue3 update

<template>
<div class='container-fluid m-2 practable-component'>
 
  <div class="row">
    <!-- LEFT PANEL-->
      <div class="d-flex flex-column col-lg-4 align-items-center justify-content-start">
          <h5>Wave Frequency</h5>

        <div class="d-flex flex-row">
            <div class="d-flex flex-column">
              <button id="frequency-step-negative-10" class="button-control-panel" aria-label="reduce frequency by 10 hertz" @click="() => {driving_frequency -= 10; sendCommandUpdateDrivingFrequency()}">-10</button>
              <button id="frequency-step-negative-1" class="button-control-panel" aria-label="reduce frequency by 1 hertz" @click="() => {driving_frequency -= 1; sendCommandUpdateDrivingFrequency()}">-1</button>
            </div>

            <seven-segment-display id="seven-segment-display-frequency" class="mb-2"
                  :value="getDrivingFrequency"
                  :reportedValue="getReportedDrivingFrequency"
                  unit="Hz"
                  fontSize="2em"
                  width="220"
                  height="220"
                  digitWidth="100"
                  digitHeight="150"
                  NDigits="2"
            />

            <div class="d-flex flex-column">
              <button id="frequency-step-positive-10" class="button-control-panel" aria-label="increase frequency by 10 hertz" @click="() => {driving_frequency += 10; sendCommandUpdateDrivingFrequency()}">+10</button>
              <button id="frequency-step-positive-1" class="button-control-panel" aria-label="increase frequency by 1 hertz" @click="() => {driving_frequency += 1; sendCommandUpdateDrivingFrequency()}">+1</button>
            </div>

        </div>


          <div class="col-12">
            <input class="" type="range" id="driving-frequency-slider" style="width: 75%;"
                :min="getDrivingFrequencyMin" 
                :max="getDrivingFrequencyMax" 
                :step="getDrivingFrequencyStep" 
                v-model="driving_frequency" 
                @mousedown="setDraggable(false)" 
                @mouseup="setDraggable(true); sendCommandUpdateDrivingFrequency()" 
                @mouseleave="setDraggable(true)">
          </div>

          
      </div>

      <!-- MIDDLE PANEL-->
      <div class="d-flex flex-column col-lg-4 align-items-center justify-content-start">
          <!-- TOGGLE FOR MODE START/STOP -->
        <div class="input-group align-items-center justify-content-center mt-lg-3 mb-lg-3">
          <label class="control-toggle-label-before" id="toggle-off-text">Off</label>
          <div class="form-check form-switch control-toggle-container">
              <input class="form-check-input" type="checkbox" id="toggle-start-button" name="toggle-start" v-model="toggleStart">
          </div> 
          <label class="control-toggle-label-after" id="toggle-on-text">On</label>
        </div>

        <!-- TOGGLE FOR MODE START/PULSE -->
        <div class="input-group align-items-center justify-content-center mt-lg-3 mb-lg-3">
          <label class="control-toggle-label-before" id="toggle-continuous-text">Steady</label>
          <div class="form-check form-switch control-toggle-container">
              <input class="form-check-input" type="checkbox" id="toggle-pulse-button" name="toggle-pulse" v-model="togglePulse">
          </div> 
          <label class="control-toggle-label-after" id="toggle-pulse-text">Pulse</label>
        </div>

        
        <button v-if="getCurrentMode == 'pulse'" id="run-pulse-button" class="button-lg button-primary mt-lg-3 mb-lg-3" aria-label="run pulse command" @click="sendCommandPulse">Start</button>
      
      </div>



      <!-- RIGHT PANEL-->
      <div class="d-flex flex-column col-lg-4 align-items-center justify-content-start">
      
        <div class="d-flex flex-row align-items-center justify-content-center">
          <div class="col">
               <label class="control-toggle-label-before me-2">Amplitude</label>
              <div class="">
                <input class="" type="range" id="amplitude-slider" style="width: 75%;"
                    :min="getAmplitudeMin" 
                    :max="getAmplitudeMax" 
                    :step="getAmplitudeStep" 
                    v-model="amplitude" 
                    @mousedown="setDraggable(false)" 
                    @mouseup="setDraggable(true); sendCommandUpdateAmplitude()" 
                    @mouseleave="setDraggable(true)">
              </div>
          </div>
             
              <div v-if="isMobile" class="d-flex flex-column mt-2">
                <button id="amplitude-step-negative-10" class="button-control-panel-small" aria-label="decrease amplitude by 10 percent" @click="() => {amplitude -= Math.floor(10*getAmplitudeMax/100); sendCommandUpdateAmplitude()}">-10</button>
                <button id="amplitude-step-negative-1" class="button-control-panel-small" aria-label="decrease amplitude by 1 percent" @click="() => {amplitude -= Math.floor(getAmplitudeMax/100); sendCommandUpdateAmplitude()}">-1</button>
              </div>
              <div v-else class="d-flex flex-column mt-2">
                <button id="amplitude-step-positive-1" class="button-control-panel-small" aria-label="increase amplitude by 1 percent" @click="() => {amplitude += Math.floor(getAmplitudeMax/100); sendCommandUpdateAmplitude()}">+</button>
                <button id="amplitude-step-negative-1" class="button-control-panel-small" aria-label="decrease amplitude by 1 percent" @click="() => {amplitude -= Math.floor(getAmplitudeMax/100); sendCommandUpdateAmplitude()}">-</button>
              </div>

              <seven-segment-display id="seven-segment-display-amplitude" 
                  :value="100*getAmplitude/getAmplitudeMax"
                  :reportedValue="100*getReportedAmplitude/getAmplitudeMax"
                  unit="%"
                  fontSize="1em"
                  width="100"
                  height="90"
                  digitWidth="25"
                  digitHeight="60"
                  NDigits="3"
                />

                <div v-if="isMobile" class="d-flex flex-column mt-2">
                <button id="amplitude-step-positive-10" class="button-control-panel-small" aria-label="increase amplitude by 10 percent" @click="() => {amplitude += Math.floor(10*getAmplitudeMax/100); sendCommandUpdateAmplitude()}">+10</button>
                <button id="amplitude-step-positive-1" class="button-control-panel-small" aria-label="increase amplitude by 1 percent" @click="() => {amplitude += Math.floor(getAmplitudeMax/100); sendCommandUpdateAmplitude()}">+1</button>
              </div>
        </div>
          

        <div class="d-flex flex-row align-items-center justify-content-center mt-2">
          <div class="col">
              <label class="control-toggle-label-before me-2">Brightness</label>
              <div class="">
                <input class="" type="range" id="brightness-slider" style="width: 75%;"
                    :min="getBrightnessMin" 
                    :max="getBrightnessMax" 
                    :step="getBrightnessStep" 
                    v-model="brightness" 
                    @mousedown="setDraggable(false)" 
                    @mouseup="setDraggable(true); sendCommandUpdateBrightness()" 
                    @mouseleave="setDraggable(true)">
              </div>
          </div>
              
              <div v-if='isMobile' class="d-flex flex-column mt-2">
                <button id="brightness-step-negative-10" class="button-control-panel-small" aria-label="decrease brightness by 10 percent" @click="() => {brightness -= Math.floor(10*getBrightnessMax/100); sendCommandUpdateBrightness()}">-10</button>
                <button id="brightness-step-negative-1" class="button-control-panel-small" aria-label="decrease brightness by 1 percent" @click="() => {brightness -= Math.floor(getBrightnessMax/100); sendCommandUpdateBrightness()}">-1</button>
              </div>
              <div v-else class="d-flex flex-column mt-2">
                <button id="brightness-step-positive-1" class="button-control-panel-small" aria-label="increase brightness by 1 percent" @click="() => {brightness += Math.floor(getBrightnessMax/100); sendCommandUpdateBrightness()}">+</button>
                <button id="brightness-step-negative-1" class="button-control-panel-small" aria-label="decrease brightness by 1 percent" @click="() => {brightness -= Math.floor(getBrightnessMax/100); sendCommandUpdateBrightness()}">-</button>
              </div>

              <seven-segment-display id="seven-segment-display-brightness" 
                  :value="100*getBrightness/getBrightnessMax"
                  :reportedValue="100*getReportedBrightness/getBrightnessMax"
                  unit="%"
                  fontSize="1em"
                  width="100"
                  height="90"
                  digitWidth="25"
                  digitHeight="60"
                  NDigits="3"
                />

                <div v-if='isMobile' class="d-flex flex-column mt-2">
                <button id="brightness-step-positive-10" class="button-control-panel-small" aria-label="increase brightness by 10 percent" @click="() => {brightness += Math.floor(10*getBrightnessMax/100); sendCommandUpdateBrightness()}">+10</button>
                <button id="brightness-step-positive-1" class="button-control-panel-small" aria-label="increase brightness by 1 percent" @click="() => {brightness += Math.floor(getBrightnessMax/100); sendCommandUpdateBrightness()}">+1</button>
              </div>
        </div>

        <div v-if="getCurrentMode == 'pulse'" >
                  <div class="d-flex flex-row align-items-center justify-content-center mt-2">
                    <div class="col">
                      <label class="control-toggle-label-before">Pulse Length</label>
                      <div class="">
                        <input class="" type="range" id="pulse-length-slider" style="width: 75%;"
                          :min="getPulseLengthMin" 
                          :max="getPulseLengthMax" 
                          :step="getPulseLengthStep" 
                          v-model="pulse_length" 
                          @mousedown="setDraggable(false)" 
                          @mouseup="setDraggable(true)" 
                          @mouseleave="setDraggable(true)">
                      </div>
                    </div>
                      
                      
                      <div v-if="isMobile" class="d-flex flex-column mt-2">
                        <button id="pulse-step-negative-10" class="button-control-panel-small" aria-label="decrease pulse by 10 ms" @click="pulse_length -= 10">-10</button>
                        <button id="pulse-step-negative-1" class="button-control-panel-small" aria-label="decrease pulse by 1 ms" @click="pulse_length -= 1">-1</button>
                      </div>
                      <div v-else class="d-flex flex-column mt-2">
                        <button id="pulse-step-positive-1" class="button-control-panel-small" aria-label="increase pulse by 1 ms" @click="pulse_length += 1">+</button>
                        <button id="pulse-step-negative-1" class="button-control-panel-small" aria-label="decrease pulse by 1 ms" @click="pulse_length -= 1">-</button>
                      </div>
                      

                      <seven-segment-display id="seven-segment-display-pulse" 
                          :value="getPulseLength"
                          :reportedValue="getPulseLength"
                          unit="ms"
                          fontSize="1em"
                          width="100"
                          height="90"
                          digitWidth="25"
                          digitHeight="60"
                          NDigits="3"
                      />

                      <div v-if="isMobile" class="d-flex flex-column mt-2">
                        <button id="pulse-step-positive-10" class="button-control-panel-small" aria-label="increase pulse by 10 ms" @click="pulse_length += 10">+10</button>
                        <button id="pulse-step-positive-1" class="button-control-panel-small" aria-label="increase pulse by 1 ms" @click="pulse_length += 1">+1</button>
                      </div>

                  </div>

          </div>

      </div>

  </div>

   <div class="d-flex flex-row align-items-start justify-content-start">
      <popup-help class="me-2" id="popup-help-control-panel">
            <template v-slot:header>
                <h5> Control Panel Help </h5>
            </template>
            <template v-slot:body>
                Control panel help
            </template>
        </popup-help>
  </div>

</div>
</template>

<script>
import { mapGetters, mapActions } from 'vuex';
import PopupHelp from './elements/PopupHelp.vue';
import SevenSegmentDisplay from './elements/SevenSegmentDisplay.vue';

export default {

  name: 'ControlPanel',
  data () {
    return {
        message: "",
        received_messages: [],
    }
  },
  components: {
    PopupHelp,
    SevenSegmentDisplay
  },
  computed:{
    ...mapGetters([
      'getDataSocket',
      'getDataURLObtained',
      'getDataURL',
      'getCurrentMode',
      'getDrivingFrequency',
      'getDrivingFrequencyMin',
      'getDrivingFrequencyMax',
      'getDrivingFrequencyStep',
      'getReportedDrivingFrequency',
      'getPulseLength',
      'getPulseLengthMin',
      'getPulseLengthMax',
      'getPulseLengthStep',
      'getAmplitude',
      'getAmplitudeMin',
      'getAmplitudeMax',
      'getAmplitudeStep',
      'getReportedAmplitude',
      'getBrightness',
      'getBrightnessMin',
      'getBrightnessMax',
      'getBrightnessStep',
      'getReportedBrightness',
      'isMobile'
    ]),
    driving_frequency: {
			get(){
				return this.getDrivingFrequency;
			},
			set(val){
				this.updateDrivingFrequency(val);
			}
		},
    pulse_length: {
			get(){
				return this.getPulseLength;
			},
			set(val){
				this.updatePulseLength(val);
			}
		},
    amplitude: {
			get(){
				return this.getAmplitude;
			},
			set(val){
				this.updateAmplitude(val);
			}
		},
    brightness: {
			get(){
				return this.getBrightness;
			},
			set(val){
				this.updateBrightness(val);
			}
		},
    toggleStart: {
			get(){
				return this.getCurrentMode != 'stop'
			},
			set(val){
        if(val){
          this.setModeStart();
          this.sendCommandStart();
        } else{
          this.setModeStop();
          this.sendCommandStop();
        }
				
			}
		},
     togglePulse: {
			get(){
				return this.getCurrentMode == 'pulse'
			},
			set(val){
        if(val){
          this.setModePulse();
          //this.sendCommandPulse();
        } else{
          this.setModeStart();
          this.sendCommandStart();
        }
				
			}
		},
    
  },
  watch:{
    getDataURLObtained(obtained){
        try{
          if(obtained){
            this.connect();	
          } else{
            console.log('disconnecting: ');
          }
				
			} catch(e){
				console.log(e);
			}
		},
  },
  created(){
      
  },
  mounted(){
      

  },
  methods: {
    ...mapActions([
        'setDraggable',
        'updateDrivingFrequency',
        'sendCommandUpdateDrivingFrequency',
        'setModeStart',
        'setModeStop',
        'setModePulse',
        'sendCommandStart',
        'sendCommandStop',
        'sendCommandPulse',
        'updatePulseLength',
        'updateAmplitude',
        'sendCommandUpdateAmplitude',
        'updateBrightness',
        'sendCommandUpdateBrightness'
        
    ]),
      sendMessage(){
        //this.getDataSocket.send(JSON.stringify(this.message));
        this.getDataSocket.send(this.message);
      },
      clearMessage(){
        this.message = ''
      },
      connect(){
          let _this = this;
          this.dataSocket = new WebSocket(this.getDataURL);
          this.$store.dispatch('setDataSocket', this.dataSocket);
          this.dataSocket.onopen = () =>  {
             
          };

          this.dataSocket.onmessage = (event) =>  {
              try {
                  
                let response = JSON.parse(event.data);

                  if(_this.received_messages.length < 10){
                    _this.received_messages.push(response);
                  } else{
                    _this.received_messages.push(response);
                    _this.received_messages.splice(0,1);
                  }
      
              } catch (e) {
                  console.log(e)
              }
          }
      },
  }
}
</script>

<style scoped>
/* LARGE SCREENS */
@media only screen and (min-width: 992px){
.control-toggle-container{
  width: 4em;
  height: 2em;
}

.control-toggle-label-before{
  font-size: 1.25em;
  width: 8ch;
  padding-right: 0.5em;
  text-align: right;
}

.control-toggle-label-after{
  font-size: 1.25em;
  width: 8ch;
  padding-left: 0.5em;
  text-align: left;
}

#toggle-pulse-button{
  width: 4em;
  height:2em;
}
#toggle-start-button{
  width: 4em;
  height:2em;
}

.button-control-panel{
    background-color: var(--background-color-secondary);
    color: var(--text-color);
    border-width: 1px;
    border-style: solid;
    border-color: var(--button-color-secondary);
    border-radius: 4px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 1.5rem;
    margin: 8px 8px;
    padding: 0px 0px;
    transition-duration: 0.2s;
    cursor: pointer;
    width: 80px;
    height: 80px;
    min-width: 40px;
    min-height: 40px;
    max-height: 80px;
    max-width: 80px;
}

.button-control-panel:hover{
    background-color: var(--button-color-hover);
    color: var(--text-color-hover);
}

.button-control-panel-small{
    background-color: var(--background-color-secondary);
    color: var(--text-color);
    border-width: 1px;
    border-style: solid;
    border-color: var(--button-color-secondary);
    border-radius: 4px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 1rem;
    margin: 4px 4px;
    padding: 0px 0px;
    transition-duration: 0.2s;
    cursor: pointer;
    width: 40px;
    height: 40px;
    min-width: 40px;
    min-height: 40px;
    max-height: 60px;
    max-width: 60px;
}

.button-control-panel-small:hover{
    background-color: var(--button-color-hover);
    color: var(--text-color-hover);
}

}


/* MOBILE SCREENS */
@media only screen and (max-width: 992px){

  /* No slider on mobile screens, just buttons to change freq*/
#driving-frequency-slider{
  display: none;
}

#pulse-length-slider{
  display: none;
}

#amplitude-slider{
  display: none;
}

#brightness-slider{
  display: none;
}

.control-toggle-container{
  width: 2em;
  height: 1em;
}

.control-toggle-label-before{
  font-size: 1em;
  width: 8ch;
  padding-right: 0.5em;
  text-align: right;
}

.control-toggle-label-after{
  font-size: 1em;
  width: 8ch;
  padding-left: 0.5em;
  text-align: left;
}

#toggle-pulse-button{
  width: 2em;
  height:1em;
}

#toggle-start-button{
  width: 2em;
  height:1em;
}

.button-control-panel{
    background-color: var(--background-color-secondary);
    color: var(--text-color);
    border-width: 1px;
    border-style: solid;
    border-color: var(--button-color-secondary);
    border-radius: 4px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 1rem;
    margin: 4px 4px;
    padding: 0px 0px;
    transition-duration: 0.2s;
    cursor: pointer;
    width: 60px;
    height: 60px;
    min-width: 40px;
    min-height: 40px;
    max-height: 60px;
    max-width: 60px;
}

.button-control-panel:hover{
    background-color: var(--button-color-hover);
    color: var(--text-color-hover);
}

.button-control-panel-small{
    background-color: var(--background-color-secondary);
    color: var(--text-color);
    border-width: 1px;
    border-style: solid;
    border-color: var(--button-color-secondary);
    border-radius: 4px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 1rem;
    margin: 4px 4px;
    padding: 0px 0px;
    transition-duration: 0.2s;
    cursor: pointer;
    width: 40px;
    height: 40px;
    min-width: 30px;
    min-height: 30px;
    max-height: 60px;
    max-width: 60px;
}

.button-control-panel-small:hover{
    background-color: var(--button-color-hover);
    color: var(--text-color-hover);
}


}






</style>
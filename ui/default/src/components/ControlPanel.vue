//Vue3 update

<template>
<div class='container-fluid m-2 practable-component'>

  <div class="row">
      <!-- LEFT PANEL-->
      <div class="d-flex flex-column col-lg-6 align-items-start justify-content-end">
        <!-- TOGGLE FOR MODE START/STOP -->
        <div class="input-group align-items-center justify-content-center">
          <label class="control-toggle-label-before" id="toggle-off-text">Off</label>
          <div class="form-check form-switch control-toggle-container">
              <input class="form-check-input" type="checkbox" id="toggle-start-button" name="toggle-start" v-model="toggleStart">
          </div> 
          <label class="control-toggle-label-after" id="toggle-on-text">On</label>
        </div>

        <!-- TOGGLE FOR MODE START/PULSE -->
        <div class="input-group align-items-center justify-content-center">
          <label class="control-toggle-label-before" id="toggle-continuous-text">Continuous</label>
          <div class="form-check form-switch control-toggle-container">
              <input class="form-check-input" type="checkbox" id="toggle-pulse-button" name="toggle-pulse" v-model="togglePulse">
          </div> 
          <label class="control-toggle-label-after" id="toggle-pulse-text">Pulse</label>
        </div>
           

          <div>Amplitude control slider</div>
          <div>brightness control slider</div>

          <div class="mt-auto">
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


      <!-- RIGHT PANEL-->
      <div class="d-flex flex-column col-lg-6 align-items-center justify-content-center">

        <div class="d-flex flex-row">
            <div class="d-flex flex-column">
              <button id="frequency-step-negative-10" class="button-control-panel" aria-label="reduce frequency by 10 hertz" @click="() => {driving_frequency -= 10; sendCommandUpdateDrivingFrequency()}">-10</button>
              <button id="frequency-step-negative-1" class="button-control-panel" aria-label="reduce frequency by 1 hertz" @click="() => {driving_frequency -= 1; sendCommandUpdateDrivingFrequency()}">-1</button>
            </div>

           <div class="seven-segment-display align-items-center justify-content-center">
          <h2 class="seven-segment-display-unit me-1">Hz</h2>
          <div class="d-flex flex-row mt-0">
              <div class="seven-segment-digit" id="seven-segment-digit-one" :style="displayColour">
              <svg xmlns="http://www.w3.org/2000/svg" class="digit" viewBox="0 -1 12 20">
                  <g class="digit">
                    <polygon id="a" :class="[2,3,5,6,7,8,9].includes(Math.floor(getDrivingFrequency/10)) ? 'on' : 'off'" points=" 1, 1  2, 0  8, 0  9, 1  8, 2  2, 2"/>
                    <polygon id="b" :class="[1,2,3,4,7,8,9].includes(Math.floor(getDrivingFrequency/10)) ? 'on' : 'off'" points=" 9, 1 10, 2 10, 8  9, 9  8, 8  8, 2"/>
                    <polygon id="c" :class="[1,3,4,5,6,7,8,9].includes(Math.floor(getDrivingFrequency/10)) ? 'on' : 'off'" points=" 9, 9 10,10 10,16  9,17  8,16  8,10"/>
                    <polygon id="d" :class="[2,3,5,6,8,9].includes(Math.floor(getDrivingFrequency/10)) ? 'on' : 'off'" points=" 9,17  8,18  2,18  1,17  2,16  8,16"/>
                    <polygon id="e" :class="[2,6,8].includes(Math.floor(getDrivingFrequency/10)) ? 'on' : 'off'" points=" 1,17  0,16  0,10  1, 9  2,10  2,16"/>
                    <polygon id="f" :class="[4,5,6,8,9].includes(Math.floor(getDrivingFrequency/10)) ? 'on' : 'off'" points=" 1, 9  0, 8  0, 2  1, 1  2, 2  2, 8"/>
                    <polygon id="g" :class="[2,3,4,5,6,8,9].includes(Math.floor(getDrivingFrequency/10)) ? 'on' : 'off'" points=" 1, 9  2, 8  8, 8  9, 9  8,10  2,10"/>
                  </g>
              </svg>
            </div>
            <div class="seven-segment-digit" id="seven-segment-digit-two" :style="displayColour">
              <svg xmlns="http://www.w3.org/2000/svg" class="digit" viewBox="0 -1 12 20">
                  <g class="digit">
                    <polygon id="a" :class="[0,2,3,5,6,7,8,9].includes(getDrivingFrequency % 10) ? 'on' : 'off'" points=" 1, 1  2, 0  8, 0  9, 1  8, 2  2, 2"/>
                    <polygon id="b" :class="[0,1,2,3,4,7,8,9].includes(getDrivingFrequency % 10) ? 'on' : 'off'" points=" 9, 1 10, 2 10, 8  9, 9  8, 8  8, 2"/>
                    <polygon id="c" :class="[0,1,3,4,5,6,7,8,9].includes(getDrivingFrequency % 10) ? 'on' : 'off'" points=" 9, 9 10,10 10,16  9,17  8,16  8,10"/>
                    <polygon id="d" :class="[0,2,3,5,6,8,9].includes(getDrivingFrequency % 10) ? 'on' : 'off'" points=" 9,17  8,18  2,18  1,17  2,16  8,16"/>
                    <polygon id="e" :class="[0,2,6,8].includes(getDrivingFrequency % 10) ? 'on' : 'off'" points=" 1,17  0,16  0,10  1, 9  2,10  2,16"/>
                    <polygon id="f" :class="[0,4,5,6,8,9].includes(getDrivingFrequency % 10) ? 'on' : 'off'" points=" 1, 9  0, 8  0, 2  1, 1  2, 2  2, 8"/>
                    <polygon id="g" :class="[2,3,4,5,6,8,9].includes(getDrivingFrequency % 10) ? 'on' : 'off'" points=" 1, 9  2, 8  8, 8  9, 9  8,10  2,10"/>
                  </g>
              </svg>
            </div>
          </div>
          </div>

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
          
          
          <div v-if="getCurrentMode == 'pulse'" class="col-12">
         
              <input class="" type="range" id="pulse-length-slider" style="width: 75%;"
                :min="getPulseLengthMin" 
                :max="getPulseLengthMax" 
                :step="getPulseLengthStep" 
                v-model="pulse_length" 
                @mousedown="setDraggable(false)" 
                @mouseup="setDraggable(true)" 
                @mouseleave="setDraggable(true)">
          
              {{ pulse_length }}

               <button id="run-pulse-button" class="button-sm button-primary" aria-label="run pulse command" @click="sendCommandPulse">Pulse</button>
              
       
            
          </div>


      </div>

  </div>

</div>
</template>

<script>
import { mapGetters, mapActions } from 'vuex';
import PopupHelp from './elements/PopupHelp.vue';

export default {

  name: 'ControlPanel',
  data () {
    return {
        message: "",
        received_messages: [],
    }
  },
  components: {
    PopupHelp
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
      'getPulseLengthStep'
    ]),
    displayColour(){
      return{
        '--on-colour': this.getDrivingFrequency == this.getReportedDrivingFrequency ? 'lightgreen' : 'orange'
      }
    },
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
        'updatePulseLength'
        
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
  font-size: 1.5em;
  width: 10ch;
  padding-right: 0.5em;
  text-align: right;
}

.control-toggle-label-after{
  font-size: 1.5em;
  width: 10ch;
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

.seven-segment-display{
  /* margin-top: 2em; */
  /* margin-bottom: 2em; */
  padding-left: 1em;
  width: 220px;
  height: 220px;
  background-color : #6d6b6b;
  border-style: dashed;
  border-color: darkgray;
}

.seven-segment-display-unit{
  font-size: 2em;
  color : #3a3636;
  font-style: italic;
  text-align: right;
}

.digit{
  width:100px;
  height:150px;
}

}


/* MOBILE SCREENS */
@media only screen and (max-width: 992px){

  /* No slider on mobile screens, just buttons to change freq*/
#driving-frequency-slider{
  display: none;
}

.control-toggle-container{
  width: 2em;
  height: 1em;
}

.control-toggle-label-before{
  font-size: 1em;
  width: 10ch;
  padding-right: 0.5em;
  text-align: right;
}

.control-toggle-label-after{
  font-size: 1em;
  width: 10ch;
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

.seven-segment-display{
  /* margin-top: 2em; */
  /* margin-bottom: 2em; */
  padding-left: 1em;
  width: 150px;
  height: 150px;
  background-color : #6d6b6b;
  border-style: dashed;
  border-color: darkgray;
}

.seven-segment-display-unit{
  font-size: 1em;
  color : #3a3636;
  font-style: italic;
  text-align: right;
}

.digit{
  width:66px;
  height:100px;
}
}


.off{
    fill: #6d6b6b;
}

.on{
    fill: var(--on-colour);
}



</style>
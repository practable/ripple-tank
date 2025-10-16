<template>
<div :id='id' :style="containerStyle">
    
         <div class="seven-segment-display align-items-center justify-content-center">
          <h2 class="seven-segment-display-unit me-1" :style="displayStyle">{{ unit }}</h2>
          <div class="d-flex flex-row">
                <div v-if="Number(NDigits) > 2" class="seven-segment-digit" id="seven-segment-digit-hundreds" :style="displayStyle">
                <svg xmlns="http://www.w3.org/2000/svg" class="digit" viewBox="0 -1 12 20">
                    <g>
                        <polygon id="a" :class="[0,2,3,5,6,7,8,9].includes(getHundredsDigit) ? 'on' : 'off'" points=" 1, 1  2, 0  8, 0  9, 1  8, 2  2, 2"/>
                        <polygon id="b" :class="[0,1,2,3,4,7,8,9].includes(getHundredsDigit) ? 'on' : 'off'" points=" 9, 1 10, 2 10, 8  9, 9  8, 8  8, 2"/>
                        <polygon id="c" :class="[0,1,3,4,5,6,7,8,9].includes(getHundredsDigit) ? 'on' : 'off'" points=" 9, 9 10,10 10,16  9,17  8,16  8,10"/>
                        <polygon id="d" :class="[0,2,3,5,6,8,9].includes(getHundredsDigit) ? 'on' : 'off'" points=" 9,17  8,18  2,18  1,17  2,16  8,16"/>
                        <polygon id="e" :class="[0,2,6,8].includes(getHundredsDigit) ? 'on' : 'off'" points=" 1,17  0,16  0,10  1, 9  2,10  2,16"/>
                        <polygon id="f" :class="[0,4,5,6,8,9].includes(getHundredsDigit) ? 'on' : 'off'" points=" 1, 9  0, 8  0, 2  1, 1  2, 2  2, 8"/>
                        <polygon id="g" :class="[2,3,4,5,6,8,9].includes(getHundredsDigit) ? 'on' : 'off'" points=" 1, 9  2, 8  8, 8  9, 9  8,10  2,10"/>
                    </g>
                </svg>
                </div>
              <div v-if="Number(NDigits) > 1" class="seven-segment-digit" id="seven-segment-digit-tens" :style="displayStyle">
              <svg xmlns="http://www.w3.org/2000/svg" class="digit" viewBox="0 -1 12 20">
                  <g>
                    <polygon id="a" :class="[0,2,3,5,6,7,8,9].includes(getTensDigit) ? 'on' : 'off'" points=" 1, 1  2, 0  8, 0  9, 1  8, 2  2, 2"/>
                    <polygon id="b" :class="[0,1,2,3,4,7,8,9].includes(getTensDigit) ? 'on' : 'off'" points=" 9, 1 10, 2 10, 8  9, 9  8, 8  8, 2"/>
                    <polygon id="c" :class="[0,1,3,4,5,6,7,8,9].includes(getTensDigit) ? 'on' : 'off'" points=" 9, 9 10,10 10,16  9,17  8,16  8,10"/>
                    <polygon id="d" :class="[0,2,3,5,6,8,9].includes(getTensDigit) ? 'on' : 'off'" points=" 9,17  8,18  2,18  1,17  2,16  8,16"/>
                    <polygon id="e" :class="[0,2,6,8].includes(getTensDigit) ? 'on' : 'off'" points=" 1,17  0,16  0,10  1, 9  2,10  2,16"/>
                    <polygon id="f" :class="[0,4,5,6,8,9].includes(getTensDigit) ? 'on' : 'off'" points=" 1, 9  0, 8  0, 2  1, 1  2, 2  2, 8"/>
                    <polygon id="g" :class="[2,3,4,5,6,8,9].includes(getTensDigit) ? 'on' : 'off'" points=" 1, 9  2, 8  8, 8  9, 9  8,10  2,10"/>
                  </g>
              </svg>
            </div>
            <div v-if="Number(NDigits) > 0" class="seven-segment-digit" id="seven-segment-digit-ones" :style="displayStyle">
              <svg xmlns="http://www.w3.org/2000/svg" class="digit" viewBox="0 -1 12 20">
                  <g>
                    
                     <polygon id="a" :class="[0,2,3,5,6,7,8,9].includes(getUnitsDigit) ? 'on' : 'off'" points=" 1, 1  2, 0  8, 0  9, 1  8, 2  2, 2"/>
                    <polygon id="b" :class="[0,1,2,3,4,7,8,9].includes(getUnitsDigit) ? 'on' : 'off'" points=" 9, 1 10, 2 10, 8  9, 9  8, 8  8, 2"/>
                    <polygon id="c" :class="[0,1,3,4,5,6,7,8,9].includes(getUnitsDigit) ? 'on' : 'off'" points=" 9, 9 10,10 10,16  9,17  8,16  8,10"/>
                    <polygon id="d" :class="[0,2,3,5,6,8,9].includes(getUnitsDigit) ? 'on' : 'off'" points=" 9,17  8,18  2,18  1,17  2,16  8,16"/>
                    <polygon id="e" :class="[0,2,6,8].includes(getUnitsDigit) ? 'on' : 'off'" points=" 1,17  0,16  0,10  1, 9  2,10  2,16"/>
                    <polygon id="f" :class="[0,4,5,6,8,9].includes(getUnitsDigit) ? 'on' : 'off'" points=" 1, 9  0, 8  0, 2  1, 1  2, 2  2, 8"/>
                    <polygon id="g" :class="[2,3,4,5,6,8,9].includes(getUnitsDigit) ? 'on' : 'off'" points=" 1, 9  2, 8  8, 8  9, 9  8,10  2,10"/>
                  </g>
              </svg>
            </div>
          </div>
          </div>
</div>
</template>

<script>
import { mapGetters } from 'vuex';

export default {

  name: 'SevenSegmentDisplay',
  props:{
      'id': String,
      'value': Number,          //the numerical value to be displayed
      'reportedValue': Number,  //the value reported by hardware to compare to value
      'width': String,          //dimensions are for the large screen versions, these will be reduced on mobiles
      'height': String,
      'digitWidth': String,
      'digitHeight': String,
      'unit': String,
      'fontSize': String,   //in em, either 1em or 2em most likely
      'NDigits': String,     //the number of digits to display, 3 digit max
  },
  data () {
    return {
        
    }
  },
  components: {
    
  },
  created(){
      
  },
  mounted(){
    

  },
  computed:{
    ...mapGetters([
        'isMobile'
    ]),
    containerStyle(){
        return{
        'width': this.isMobile ? String(Number(this.width)*4/5) + 'px' : this.width + 'px',
        'height': this.isMobile ? String(Number(this.height)*4/5) + 'px' : this.height + 'px'
      }
    },
   displayStyle(){
      return{
        '--on-colour': this.value == this.reportedValue ? 'lightgreen' : 'orange',
        '--display-width': this.isMobile ? String(Number(this.width)*4/5) + 'px' : this.width + 'px',
        '--display-height': this.isMobile ? String(Number(this.height)*4/5) + 'px' : this.height + 'px',
        '--digit-width': this.isMobile ? String(Number(this.digitWidth)*4/5) + 'px' : this.digitWidth + 'px',
        '--digit-height': this.isMobile ? String(Number(this.digitHeight)*4/5) + 'px' : this.digitHeight + 'px',
        '--font-size': this.fontSize
      }
    },
    getHundredsDigit(){
        //values cannot be larger than 999
        return Math.floor(this.value/100);
    },
    getTensDigit(){
        if(this.value > 99){
            return Math.floor(this.value/10) % 10;
        } else {
            return Math.floor(this.value/10);
        }
    },
    getUnitsDigit(){
        return Math.floor(this.value % 10)
    }
  },
  watch:{
      
  },
  
  methods: {
      
      
      
      
      
  }
}
</script>

<style scoped>

.seven-segment-display{
  /* margin-top: 2em; */
  /* margin-bottom: 2em; */
  padding-left: 1em;
  /* width: var(--display-width);
  height: var(--display-height); */
  background-color : #6d6b6b;
  border-style: dashed;
  border-color: darkgray;
}

.seven-segment-display-unit{
  font-size: var(--font-size);
  color : #3a3636;
  font-style: italic;
  text-align: right;
}

.digit{
  width:var(--digit-width);
  height:var(--digit-height);
}

.off{
    fill: #6d6b6b;
}

.on{
    fill: var(--on-colour);
}

</style>
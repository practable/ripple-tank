//Vue3 update

<template>
<div class='container-fluid m-2 practable-component table'>

  <div class="row "><h2>Control Panel</h2></div>

    <div class="d-flex flex-row">
      <input id="input-command" type="text" class="form-control input" aria-label="command input" v-model="message">
      <button id="send-button" class="button-lg button-primary" @click="sendMessage">Send</button>
      <button id="clear-button" class="button-lg button-warning" @click="clearMessage">Clear</button>
    </div>

    <div class="row "><h2>Received Data</h2></div>

      <div v-for="message in received_messages" class="row">
          <p>{{ message }}</p>
      </div>

</div>
</template>

<script>
import { mapGetters } from 'vuex';

export default {

  name: 'ControlPanel',
  data () {
    return {
        message: "",
        received_messages: [],
    }
  },
  components: {
    
  },
  computed:{
    ...mapGetters([
      'getDataSocket',
      'getDataURLObtained',
      'getDataURL'
    ])
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
//console.log(this.dataSocket);
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


</style>
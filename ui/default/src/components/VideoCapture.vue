//https://developer.mozilla.org/en-US/docs/Web/API/MediaStream_Recording_API

<template>
  <div class='container-fluid practable-component'>

    <div class="row">
        <div class="col-lg-8">
            <div class="input-group mb-3" @mousedown="setDraggable(false)" @mouseup="setDraggable(true)">
                <label for="recording-length-slider">Record Time: {{ this.captureInterval/1000 }} s</label>
                <input type="range" id="recording-length-slider" class="slider" step="100" min="100" max="3000" v-model="captureInterval">
            </div>
            <div class="input-group mb-3" @mousedown="setDraggable(false)" @mouseup="setDraggable(true)">
                <label for="recording-fps-slider">FPS: {{ this.framerate }}</label>
                <input type="range" id="recording-fps-slider" class="slider" step="1" min="1" max="24" v-model="framerate">
            </div>
        </div>

        <div class="col-lg-4 d-flex align-items-center justify-content-center">
            <button v-if="!isRecording" id='recordButton' class="button-lg button-primary" @click="capture">Record</button>
            <div v-if="isRecording" class="spinner-border text-primary" role="status">
                <span class="visually-hidden">Loading...</span>
            </div>
        </div>
        
    </div>
    
    
    <div class="row">
        <div class="video-container col-12" @mouseenter="setDraggable(false)" @mouseleave="setDraggable(true)">
            <video id="recording" controls></video>
        </div>
        
    </div>

    <div class="d-flex align-items-center justify-content-center">
        <button id="frameBackwardButton" class="button-sm button-primary" @click="frameBackward">Step Backward</button>
        <button id="frameForwardButton" class="button-sm button-primary" @click="frameForward">Step Forward</button>
        <!-- <label for="frameForwardButton">Video time: {{ frame_time.toFixed(3) }}</label> -->
        <!-- <label v-if="video != null" for="frameForwardButton">Video time: {{ getCurrentVideoTime }}</label> -->
    </div>

    <div class="d-flex justify-content-center">
        <button id="downloadButton" class="button-sm button-primary" @click="download">Download</button>
    </div>
    
    
    
    

      
  </div>
</template>

<script>
import { mapGetters, mapActions } from 'vuex';

export default {

  name: 'VideoCapture',
  data () {
    return {
        recordedChunks: [],
        captureInterval: 2000,
        framerate: 24,
        isRecording: false,
        video: null,     //{video: src, framerate: fps, length: length}
        frame_time: 0,
    }
  },
  components: {
    
  },
  computed:{
    ...mapGetters([
        

        
    ]),
    getCurrentVideoTime(){
        if(this.video != null){
            console.log(this.video.video.currentTime)
            return this.video.video.currentTime;
        } else{
            return 0;
        }
        
    }
  },
  watch:{
    
  },
  created(){
    window.addEventListener('keydown', (event) => {
        if(event.repeat){
            return
        } else{
            if(event.key === 'd' || event.key === 'ArrowRight'){
                this.frameForward();
            } else if(event.key === 'a' || event.key === 'ArrowLeft'){
                this.frameBackward();
            }
        }
    })
  },
  mounted(){
    

  },
  methods: {
    ...mapActions([
      'setDraggable'
    ]),
    capture(){
        //const canvas = document.querySelector("video-canvas");
        const canvas = document.getElementById("video-canvas");

        if(canvas){
            // Optional frames per second argument.
            try{
                const stream = canvas.captureStream(this.framerate);
                this.recordedChunks = [];

                //console.log(stream);
                const options = { mimeType: "video/webm" };
                const mediaRecorder = new MediaRecorder(stream, options);

                mediaRecorder.ondataavailable = this.handleDataAvailable;
                mediaRecorder.start();
                this.isRecording = true;

                setTimeout((event) => {
                    mediaRecorder.stop();
                    this.isRecording = false;
                }, this.captureInterval);

            } catch(e){
                console.log('could not capture video stream')
            }
            
            
        } else{
            console.log('no video canvas available')
        }
        
    },
    handleDataAvailable(event){
        if (event.data.size > 0) {
            this.recordedChunks.push(event.data);
            //console.log(this.recordedChunks);
            this.record();
        } else {
            console.log('not ready');
        }
    },
    record(){
        const blob = new Blob(this.recordedChunks, {
            type: "video/webm",
        });
        const url = URL.createObjectURL(blob);
        let recording = document.getElementById('recording');
        this.video = {video: recording, framerate: this.framerate, frametime: 1/this.framerate, length: this.captureInterval};
        if(recording){
            recording.src = url;
        }
    },
    download(){
        const blob = new Blob(this.recordedChunks, {
            type: "video/webm",
        });
        const url = URL.createObjectURL(blob);
        const a = document.createElement("a");
        document.body.appendChild(a);
        a.style = "display: none";
        a.href = url;
        a.download = "video.webm";
        a.click();
        window.URL.revokeObjectURL(url);
    },
    frameForward(){
        console.log('frame formward')
        this.video.video.currentTime = Math.min(this.video.video.duration, this.video.video.currentTime + this.video.frametime);
        this.frame_time = this.video.video.currentTime;
    },
    frameBackward(){
        console.log('frame backward')
        this.video.video.currentTime = Math.max(0, this.video.video.currentTime - this.video.frametime);
        this.frame_time = this.video.video.currentTime;
    }
    



      
  },
}

</script>

<style scoped>
#recording{
    width: 100%;
}

.video-container{
    display: block;
}

</style>
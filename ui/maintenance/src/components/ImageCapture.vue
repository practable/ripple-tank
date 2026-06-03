//https://developer.mozilla.org/en-US/docs/Web/API/MediaStream_Recording_API

<template>
  <div class='container-fluid practable-component'>

    <!-- Interaction Buttons -->
    <div class="row">
      <button id='captureImageButton' class="button-lg button-primary" @click="capture">Capture</button>
    </div>
    <div class="row">
      <div class="col-md-6">
        <button id="downloadImageButton" class="button-sm button-primary" @click="downloadAll">Download All</button>
      </div>
      <div class="col-md-6">
        <button id="clearImageButton" class="button-sm button-warning" @click="deleteAll">Delete All</button>
      </div>
      
    <label for="captureImageButton" id="captureImageLabel">{{ images_index }} / {{ max_captures }}</label>
    </div>
    
    <!-- Displayed, large image for selected image-->
    <div class="row">
        <div class="col-sm-12 m-2">
          <img v-if="selected_image != null" :src="selected_image.src" :alt='selected_image.alt' class="display-image"> 
          <div v-else><h2>Select an image to display it</h2></div>
        </div>
    </div>
   
   <!-- Captured images thumbnails grid-->
    <div class="row">
        <div v-for="image in images" class="thumbnail-container col-3 mb-2">
          <img :src="image.src" :alt='image.alt' :class="getImageClass(image)" @click="selected_image = image"> 

          <button type='button' class='download-icon' id='download-image-button' aria-label="download image" @click="downloadImage(image)">
            <svg xmlns="http://www.w3.org/2000/svg" width='16' height='16' fill="currentColor" class="bi bi-download" viewBox="0 0 16 16">
                <path d="M.5 9.9a.5.5 0 0 1 .5.5v2.5a1 1 0 0 0 1 1h12a1 1 0 0 0 1-1v-2.5a.5.5 0 0 1 1 0v2.5a2 2 0 0 1-2 2H2a2 2 0 0 1-2-2v-2.5a.5.5 0 0 1 .5-.5z"/>
                <path d="M7.646 11.854a.5.5 0 0 0 .708 0l3-3a.5.5 0 0 0-.708-.708L8.5 10.293V1.5a.5.5 0 0 0-1 0v8.793L5.354 8.146a.5.5 0 1 0-.708.708l3 3z"/>
            </svg>
          </button>

          <button type='button' class='delete-icon' id='delete-image-button' aria-label="delete image" @click="deleteImage(image)">
            <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-trash3" viewBox="0 0 16 16">
              <path d="M6.5 1h3a.5.5 0 0 1 .5.5v1H6v-1a.5.5 0 0 1 .5-.5M11 2.5v-1A1.5 1.5 0 0 0 9.5 0h-3A1.5 1.5 0 0 0 5 1.5v1H1.5a.5.5 0 0 0 0 1h.538l.853 10.66A2 2 0 0 0 4.885 16h6.23a2 2 0 0 0 1.994-1.84l.853-10.66h.538a.5.5 0 0 0 0-1zm1.958 1-.846 10.58a1 1 0 0 1-.997.92h-6.23a1 1 0 0 1-.997-.92L3.042 3.5zm-7.487 1a.5.5 0 0 1 .528.47l.5 8.5a.5.5 0 0 1-.998.06L5 5.03a.5.5 0 0 1 .47-.53Zm5.058 0a.5.5 0 0 1 .47.53l-.5 8.5a.5.5 0 1 1-.998-.06l.5-8.5a.5.5 0 0 1 .528-.47M8 4.5a.5.5 0 0 1 .5.5v8.5a.5.5 0 0 1-1 0V5a.5.5 0 0 1 .5-.5"/>
            </svg>
          </button>

        </div>
    </div>


      
  </div>
</template>

<script>
import { mapGetters, mapActions } from 'vuex';

export default {

  name: 'ImageCapture',
  data () {
    return {
        selected_image: null,
        images_index: 0,
        images: [],
        max_captures: 16,
    }
  },
  components: {
    
  },
  computed:{
    ...mapGetters([
        

        
    ]),
    
  },
  watch:{
    
  },
  created(){
    
    
  },
  mounted(){
    

  },
  methods: {
    ...mapActions([
      
    ]),
    getImageClass(image){
      if(image == this.selected_image){
        return "captured-image selected-image"
      } else{
        return "captured-image"
      }
    },
    capture(){
        const canvas = document.getElementById("video-canvas");
        const newImg = new Image();
        newImg.src = canvas.toDataURL("image/png")
        newImg.alt = "captured image " + this.images_index;
        if(this.images_index < this.max_captures){
          this.images.push(newImg);
          this.images_index += 1;
        } else{
          console.log('captures full')
        }
    },
    deleteAll(){
      this.images = []
      this.images_index = 0
      this.selected_image = null
    },
    downloadAll(){
        this.images.forEach(image => {
            let link = document.createElement('a');
            let date = new Date();
            link.download = date.getHours() + "_" + date.getMinutes() + "_" + image.alt + ".png";
            link.href = image.src;
            link.click();
            link.remove();
        });
    },
    downloadImage(image){
      let link = document.createElement('a');
      let date = new Date();
      link.download = date.getHours() + "_" + date.getMinutes() + "_" + image.alt + ".png";
      link.href = image.src;
      link.click();
      link.remove();
    },
    deleteImage(to_delete){
      //find the image in the array via src or alt
      //get the index
      //splice that index
      let index = this.images.findIndex((img) => img.src == to_delete.src);
      this.images.splice(index, 1);
      this.images_index -= 1;

      if(this.selected_image == to_delete){
        this.selected_image = null;
      }


    }
      
  },
}

</script>

<style scoped>
.captured-image{
    width: 100%;
}

.display-image{
    width: 100%;
}

.selected-image{
    border-style: dashed;
    border-radius:12px;
    border-width: 2px;
    border-color: var(--button-color-primary)
}

.thumbnail-container{
  position: relative;
}

.thumbnail-container img{
  display: block;
}

.thumbnail-container .download-icon{
  position: absolute;
  bottom: 0;
  left: 10%;
  opacity: 50%;
  width: 20%;
  aspect-ratio: 1/1;
  transition-duration: 0.2s;
  cursor: pointer;
  border: none;
  border-radius: 4px;
  padding: 8px 16px;
  text-align: center;
  text-decoration: none;
  display: block;
  font-size: 18px;
  margin: 0px 0px;
  padding: 0px 0px;
  
}

.thumbnail-container .delete-icon{
  position: absolute;
  bottom: 0;
  right: 10%;
  opacity: 50%;
  width: 20%;
  aspect-ratio: 1/1;
  transition-duration: 0.2s;
  cursor: pointer;
  border: none;
  border-radius: 4px;
  padding: 8px 16px;
  text-align: center;
  text-decoration: none;
  display: block;
  font-size: 18px;
  margin: 0px 0px;
  padding: 0px 0px;
  
}


</style>
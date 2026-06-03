//29/01/2026 - update to add marker tools and better control of ruler and protractor separately

<template>
<div class="d-flex align-items-center">
    <div class="col-lg-2">
        <div class="d-flex form-check form-switch ms-2">
            <input class="form-check-input me-1" type="checkbox" id="workspace-toggle" @click="toggleWorkspaceClickable" v-model="workspace_canvas_clickable">
            <label class="form-check-label" for="workspace-toggle">Toggle Tools </label>
        </div>

        <div class="d-flex align-items-center ms-2">
            <button type='button' class='button-toolbar button-primary me-2 mt-2' id='add-marker-button' aria-label='add marker button' @click='addMarker'>
            <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" stroke="currentColor" class="bi bi-plus-lg" viewBox="0 0 16 16">
                <path fill-rule="evenodd" d="M8 2a.5.5 0 0 1 .5.5v5h5a.5.5 0 0 1 0 1h-5v5a.5.5 0 0 1-1 0v-5h-5a.5.5 0 0 1 0-1h5v-5A.5.5 0 0 1 8 2"/>
            </svg>
        </button>
        <label class="form-check-label" for="add-marker-button">Add marker</label>
        </div>
        
    </div>

    <!-- COMPONENT FOR MANIPULATING SIZE AND ANGLE OF RULER -->
    <div class="col-lg-10 ms-2" style="overflow-x: scroll;">
        <div class="d-flex flex-row align-items-center ">
            <button id='rotate-ruler-anti-clockwise-large-button' class='button-sm button-primary me-2' @mousedown='rotateTool(-10, "ruler")'>
                <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M2.5 2v6h6M2.66 15.57a10 10 0 1 0 .57-8.38"/></svg>
            </button>
            <button id='rotate-ruler-anti-clockwise-small-button' class='button-sm button-primary me-2' @mousedown='rotateTool(-1, "ruler")'>
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M2.5 2v6h6M2.66 15.57a10 10 0 1 0 .57-8.38"/></svg>
            </button>

            <button id='decrease-ruler-size-large-button' class='button-sm button-primary' @click='decreaseRulerSizeBig'>
                <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" fill="currentColor" stroke="currentColor" class="bi bi-dash-lg" viewBox="0 0 16 16">
                    <path fill-rule="evenodd" d="M2 8a.5.5 0 0 1 .5-.5h11a.5.5 0 0 1 0 1h-11A.5.5 0 0 1 2 8"/>
                </svg>
            </button>
            <button id='decrease-ruler-size-small-button'class='button-sm button-primary' @click='decreaseRulerSize'>
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" stroke="currentColor" class="bi bi-dash-lg" viewBox="0 0 16 16">
                    <path fill-rule="evenodd" d="M2 8a.5.5 0 0 1 .5-.5h11a.5.5 0 0 1 0 1h-11A.5.5 0 0 1 2 8"/>
                </svg>
            </button>

            <label type='text' class='ms-2 me-2'>Ruler</label>  

            <button id='increase-ruler-size-small-button' class='button-sm button-primary me-2' @click='increaseRulerSize'>
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" stroke="currentColor" class="bi bi-plus-lg" viewBox="0 0 16 16">
                    <path fill-rule="evenodd" d="M8 2a.5.5 0 0 1 .5.5v5h5a.5.5 0 0 1 0 1h-5v5a.5.5 0 0 1-1 0v-5h-5a.5.5 0 0 1 0-1h5v-5A.5.5 0 0 1 8 2"/>
                </svg>
            </button>
            <button id='increase-ruler-size-large-button' class='button-sm button-primary me-2' @click='increaseRulerSizeBig'>
                <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" fill="currentColor" stroke="currentColor" class="bi bi-plus-lg" viewBox="0 0 16 16">
                    <path fill-rule="evenodd" d="M8 2a.5.5 0 0 1 .5.5v5h5a.5.5 0 0 1 0 1h-5v5a.5.5 0 0 1-1 0v-5h-5a.5.5 0 0 1 0-1h5v-5A.5.5 0 0 1 8 2"/>
                </svg>
            </button>

            <button id='rotate-ruler-clockwise-small-button' class='button-sm button-primary me-2' @click='rotateTool(1, "ruler")'>
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M21.5 2v6h-6M21.34 15.57a10 10 0 1 1-.57-8.38"/></svg>
            </button>
            <button id='rotate-ruler-clockwise-large-button' class='button-sm button-primary me-2' @click='rotateTool(10, "ruler")'>
                <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M21.5 2v6h-6M21.34 15.57a10 10 0 1 1-.57-8.38"/></svg>
            </button>
        </div>

        <div class="d-flex flex-row align-items-center">
            <button class='button-sm button-primary me-2' @mousedown='rotateTool(-10, "protractor")'>
                <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M2.5 2v6h6M2.66 15.57a10 10 0 1 0 .57-8.38"/></svg>
            </button>
            <button class='button-sm button-primary me-2' @mousedown='rotateTool(-1, "protractor")'>
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M2.5 2v6h6M2.66 15.57a10 10 0 1 0 .57-8.38"/></svg>
            </button>

            <button class='button-sm button-primary' @click='decreaseProtractorSize'>
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" stroke="currentColor" class="bi bi-dash-lg" viewBox="0 0 16 16">
                    <path fill-rule="evenodd" d="M2 8a.5.5 0 0 1 .5-.5h11a.5.5 0 0 1 0 1h-11A.5.5 0 0 1 2 8"/>
                </svg>
            </button>

            <label type='text' class='ms-2 me-2'>Protractor</label>  

            <button class='button-sm button-primary me-2' @click='increaseProtractorSize'>
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" stroke="currentColor" class="bi bi-plus-lg" viewBox="0 0 16 16">
                    <path fill-rule="evenodd" d="M8 2a.5.5 0 0 1 .5.5v5h5a.5.5 0 0 1 0 1h-5v5a.5.5 0 0 1-1 0v-5h-5a.5.5 0 0 1 0-1h5v-5A.5.5 0 0 1 8 2"/>
                </svg>
            </button>

            <button class='button-sm button-primary me-2' @click='rotateTool(1, "protractor")'>
                <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M21.5 2v6h-6M21.34 15.57a10 10 0 1 1-.57-8.38"/></svg>
            </button>
            <button class='button-sm button-primary me-2' @click='rotateTool(10, "protractor")'>
                <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M21.5 2v6h-6M21.34 15.57a10 10 0 1 1-.57-8.38"/></svg>
            </button>
        </div>

    </div>

    <!-- COMPONENT FOR MANIPULATING SIZE AND ANGLE OF PROTRACTOR -->
    


    <!-- <div class="col-md-2">
        <button class='button-sm button-primary me-2' @click='increaseRulerSize'>+</button>
        <input type='text' class='input-disabled' placeholder='Size (Ruler)' size='10'>  
        <button class='button-sm button-primary' @click='decreaseRulerSize'>-</button>
    </div> -->

    <canvas :class="workspace_canvas_clickable ? 'clickable' : 'unclickable'" style="touch-action: none" id="workspace" @pointerdown="checkClick" @pointermove="moveClicked" @pointerup="mouseUnclick"></canvas>

    <img id="ruler-image" src="/images/ruler.png" hidden>
    <img id="protractor" src="/images/protractor.png" hidden>
    <img id="crosshair1" src="/images/crosshair.svg" hidden>
    
</div>
</template>

<script>

var canvas;
var ctx;
let protractor = new Image();
let ruler = new Image();
let crosshair1 = new Image();
let shapes = [];        //the added objects to canvas

export default {
    name: "Workspace",
    props:{
      protractorAdded: Boolean,
      rulerAdded: Boolean
  },
    data(){
        return{
            isSelected: false,
            selected_index: null,
            selected_offset_x: 0,
            selected_offset_y: 0,
            rotateMode: false,
            workspace_canvas_clickable: true,
            ruler_width: 800,
            ruler_height: 80,
            ruler_ratio: 0.1,
            protractor_width: 400,
            protractor_height: 200,
            protractor_ratio: 0.5,
            mouseHeld: false
        }
    },
    created(){
        
    },
    mounted(){
        shapes = [];        //ensure when mounted again that the shapes are not redrawn
        canvas = document.getElementById("workspace");
        canvas.width = screen.width;
        canvas.height = screen.height;
        ctx = canvas.getContext("2d");

        //add a key press modifiers to the window
        window.addEventListener('keydown', this.updateMode, false);
        window.addEventListener('keyup', this.updateMode, false);
        //window.addEventListener('resize', () => {setTimeout(this.resizeRuler, 100)});

        //this.resizeRuler();
    },
    watch:{
        protractorAdded(set) {
            if(set){
                this.addProtractor();
            }
        },
        rulerAdded(set) {
            if(set){
                this.addRuler();
            }
        },
    },
    methods:{
        draw() {
            ctx.clearRect(0,0,screen.width, screen.height);

            for(let i=0; i< shapes.length;i++){

                ctx.save();

                if(shapes[i].image != null){
                    if(shapes[i].name == 'ruler'){
                        shapes[i].width = this.ruler_width;
                        shapes[i].height = this.ruler_height;
                    } 
                    else if(shapes[i].name == 'protractor'){
                        shapes[i].width = this.protractor_width;
                        shapes[i].height = this.protractor_height;
                    } 

                    ctx.translate(shapes[i].x,shapes[i].y);
                    ctx.translate(shapes[i].width/2, shapes[i].height/2)
                    ctx.rotate(shapes[i].angle);
                    //ctx2.drawImage(shapes[i].image, shapes[i].x, shapes[i].y, shapes[i].width, shapes[i].height);
                    ctx.drawImage(shapes[i].image, -shapes[i].width/2, -shapes[i].height/2, shapes[i].width, shapes[i].height);
                } 
                else
                {
                    // ctx2.clearRect(0,0,screen.width, screen.height);
                    ctx.beginPath();
                    ctx.rect(shapes[i].x, shapes[i].y, shapes[i].width, shapes[i].height);
                    ctx.stroke();
                    ctx.fill();
                    ctx.fillStyle = "#444444";
                }

                ctx.restore();
            }

        },
        addProtractor(){
            protractor.onload = function() {
                let x = 100;
                let y= 100;
                let w=400;
                let h=200;
                shapes.push( {x:x, y:y, width:w, height:h, image:protractor, angle:0, name:'protractor'} );
                ctx.drawImage(protractor, x, y, w, h);
                
                
            };
            protractor.src = document.getElementById("protractor").src;
        },
        addRuler(){
            let _this = this;
            ruler.onload = function() {
                let x = 100;
                let y= 100;
                let w = _this.ruler_width;
                let h = _this.ruler_height;
                
                shapes.push( {x:x, y:y, width:w, height:h, image:ruler, angle:0, name:'ruler'} );
                ctx.drawImage(ruler, x, y, w, h);
                
            };

            ruler.onerror = function(){
                console.log('no image');
            }
            
            ruler.src = document.getElementById("ruler-image").src;
        
        },
        addMarker(){
            let _this = this;
            crosshair1.onload = function() {
                let x = 100;
                let y= 100;
                let w = 50;
                let h = 50;
                
                shapes.push( {x:x, y:y, width:w, height:h, image:crosshair1, angle:0, name:'marker'} );
                ctx.drawImage(crosshair1, x, y, w, h);
                
            };

            crosshair1.onerror = function(){
                console.log('no image');
            }
            
            crosshair1.src = document.getElementById("crosshair1").src;
        },
        updateMode(event){
            if(event.repeat){
                return;
            } else{
                if(event.key == "o"){
                    this.rotateMode = !this.rotateMode;
                } 
                else if(event.key == "w" && event.type == 'keydown'){
                    console.log(event);
                    this.workspace_canvas_clickable = !this.workspace_canvas_clickable;
                }
            }
            
        },
        toggleWorkspaceClickable(){
            this.workspace_canvas_clickable = !this.workspace_canvas_clickable;
        },
        checkClick(event){
            for(let i=0; i<shapes.length;i++){

                let origin = [0,0];
                let centre = [shapes[i].x + shapes[i].width/2, shapes[i].y + shapes[i].height/2];

                //click position relative to the centre of the shape.
                let clickPos = [event.offsetX - centre[0], event.offsetY - centre[1]];
                //let clickPos = [event.offsetX, event.offsetY];

                //from the centre of the shape
                let vertices = [];
                let vertex1 = [shapes[i].x - centre[0], shapes[i].y - centre[1]];
                let vertex2 = [shapes[i].x + shapes[i].width - centre[0], shapes[i].y - centre[1]];
                let vertex3 = [shapes[i].x + shapes[i].width - centre[0], shapes[i].y + shapes[i].height - centre[1]];
                let vertex4 = [shapes[i].x - centre[0], shapes[i].y + shapes[i].height - centre[1]];
                vertices.push(vertex1);
                vertices.push(vertex2);
                vertices.push(vertex3);
                vertices.push(vertex4);

                let angle = shapes[i].angle;

                //rotate the vertices around the centre of the shape
                for(let i=0;i<4;i++){
                    let x_prev = vertices[i][0];
                    let y_prev = vertices[i][1];

                    vertices[i][0] = x_prev*Math.cos(angle) - y_prev*Math.sin(angle);
                    vertices[i][1] = x_prev*Math.sin(angle) + y_prev*Math.cos(angle);
                }
                
                if(!this.doIntersect(origin,clickPos,vertices[0], vertices[1]) && !this.doIntersect(origin,clickPos,vertices[1], vertices[2]) && !this.doIntersect(origin,clickPos,vertices[2], vertices[3]) && !this.doIntersect(origin,clickPos,vertices[3], vertices[0])){
                    this.isSelected = true;
                    this.selected_index = i;
                    this.selected_offset_x = event.offsetX - shapes[i].x;
                    this.selected_offset_y = event.offsetY - shapes[i].y;
                }

            }
            
        },
        // To find orientation of ordered triplet (p, q, r). 
        // The function returns following values 
        // 0 --> p, q and r are colinear 
        // 1 --> Clockwise 
        // 2 --> Counterclockwise 
        checkOrientation(q, p, r){
            // See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
                // for details of below formula. 
                let val = (q[1] - p[1]) * (r[0] - q[0]) - 
                        (q[0] - p[0]) * (r[1] - q[1]); 
            
                if (val == 0) return 0;  // colinear 
            
                return (val > 0) ? 1: 2; // clock or counterclock wise 
        },
        doIntersect(p1,q1,p2,q2){
            let o1 = this.checkOrientation(p1, q1, p2); 
            let o2 = this.checkOrientation(p1, q1, q2); 
            let o3 = this.checkOrientation(p2, q2, p1); 
            let o4 = this.checkOrientation(p2, q2, q1); 
  
            // General case 
            if (o1 != o2 && o3 != o4) {
                return true;
            } else{
                return false;
            }
                 
        },
        moveClicked(event){
            if(this.isSelected){
                if(this.rotateMode){
                    if(event.movementY < 0){
                        shapes[this.selected_index].angle -= Math.PI/180;
                    } else if(event.movementY > 0){
                        shapes[this.selected_index].angle += Math.PI/180;
                    }
                    
                } 
                else
                {
                    //ctx2.clearRect(rect_top_left_x, rect_top_left_y, rect_width, rect_height);
                    shapes[this.selected_index].x = event.offsetX - this.selected_offset_x;
                    shapes[this.selected_index].y = event.offsetY - this.selected_offset_y;
                
                }
                this.draw();        //redraw the shapes only when attempting to move
            }
            
        },
        mouseUnclick(){
            this.isSelected = false;
            this.selected_index = null;
        },
        // resizeRuler(){
        //     console.log('resizing');
        //     this.ruler_width = this.video_canvas.clientWidth * this.ruler_video_width_ratio;
            
        //     this.ruler_height = this.ruler_ratio*this.ruler_width;
             
        //     this.draw();
            
        // },
        increaseRulerSize(){
            this.ruler_width += 10;
            this.ruler_height = this.ruler_ratio*this.ruler_width;
            this.draw();
        },
        increaseRulerSizeBig(){
            this.ruler_width += 100;
            this.ruler_height = this.ruler_ratio*this.ruler_width;
            this.draw();
        },
        increaseProtractorSize(){
            this.protractor_width += 10;
            this.protractor_height = this.protractor_ratio*this.protractor_width;
            this.draw();
        },
        decreaseRulerSize(){
            if(this.ruler_width > 100){
                this.ruler_width -= 10;
                this.ruler_height = this.ruler_ratio*this.ruler_width;
                this.draw();
            }
        },
        decreaseRulerSizeBig(){
            if(this.ruler_width > 100){
                this.ruler_width -= 100;
                this.ruler_height = this.ruler_ratio*this.ruler_width;
                this.draw();
            }
        },
        decreaseProtractorSize(){
            if(this.protractor_width > 100){
                this.protractor_width -= 10;
                this.protractor_height = this.protractor_ratio*this.protractor_width;
                this.draw();
            }
        },
        // rotateRuler(angle){
        //     // if(shapes[0] != null){
        //     //     shapes[0].angle += angle*Math.PI/180;
        //     // }

        //     // if(shapes[1] != null){
        //     //     shapes[1].angle += angle*Math.PI/180;
        //     // }
        //     for (let i=0; i<shapes.length; i++){
        //         if(shapes[i].name == 'ruler'){
        //             shapes[i].angle += angle*Math.PI/180;
        //             break;
        //         }
        //     }
            
        //     this.draw(); 
        // },
        rotateTool(angle, tool){
            //angle is the additional angle to rotate
            // tool is a string name of the tool, either 'ruler', 'protractor' or perhaps 'marker'
            for (let i=0; i<shapes.length; i++){
                if(shapes[i].name == tool){
                    shapes[i].angle += angle*Math.PI/180;
                    break;
                }
            }
            
            this.draw(); 
        }
    }
}


</script>

<style scoped>

#workspace{
    z-index: 2;
    position:absolute;
    left:0px;
    top:200px;
    /* pointer-events: none; */
}

.unclickable{
    pointer-events: none;
    opacity: 0.0;
}

.clickable{
    pointer-events: all;
    opacity: 1;
}


</style>
"use strict";

var gl;                 // The webgl context.

var a_coords_loc;       // Location of the a_coords attribute variable in the shader program.
var a_coords_buffer;    // Buffer to hold the values for a_coords.
var a_normal_loc;       // Location of a_normal attribute.
var a_normal_buffer;    // Buffer for a_normal.
var index_buffer;       // Buffer to hold vetex indices from model.

var u_diffuseColor;     // Locations of uniform variables in the shader program
var u_specularColor;
var u_specularExponent;
var u_lightPosition;
var u_modelview;
var u_projection;
var u_normalMatrix;    
var u_carpos1;
var u_carpos2;
var u_cardir;
var u_is_sun;

var projection = mat4.create();          // projection matrix
var modelview;                           // modelview matrix; value comes from rotator
var normalMatrix = mat3.create();        // matrix, derived from model and view matrix, for transforming normal vectors
var rotator;                             // A TrackballRotator to implement rotation by mouse.

var lastTime = 0;
var colors = [  // RGB color arrays for diffuse and specular color values
    [1,1,1],
];

var lightPositions = [  // values for light position
  [0,0,0,1],
];

var objects = [         // Objects for display
    cube(),ring(),uvSphere(), uvTorus(0.35,0.35/2),uvCylinder(),uvCone(0.6,1.5,14)
];

var matrix_stack=[];
function push_matrix() {
    matrix_stack.push( mat4.clone(modelview) );
}

function pop_matrix() {
    modelview = matrix_stack.pop();
}


var currentModelNumber;  // contains data for the current object

function degToRad(degrees) {
  return degrees * Math.PI / 180;
}



     // return mat4.perspective(projection,view,number,near,far);   
      
     //return mat4.translate(m,m,vertices);
      
      //return mat4.rotate(m,m,radian,axis);
     
      //return mat4.scale(m,m,vertices);
    


var is_sun=1; //  check whether it is day or night
var rotate_degree=0;
function draw() { 
    gl.clearColor(0,0,0,1);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);


    mat4.perspective(projection,Math.PI/5,1,10,20);   
    modelview = rotator.getViewMatrix();
    if (document.getElementById("animation").checked)
    {
        rotate_degree+=0.8;
    }
    
    rotate_degree=rotate_degree%360;
    
    if (rotate_degree>180)
        is_sun=1;
    else
        {
            is_sun=0;
        }


    gl.uniform1i(u_is_sun, is_sun);

    
   /* if(is_sun!=1)
        {
        gl.uniform4fv(u_lightPosition, [0,2,0,1]);
        }
    else
        gl.uniform4fv(u_lightPosition, [-Math.sin(degToRad(rotate_degree*2)), 0, -Math.cos(degToRad(rotate_degree*2)),0]);
    */
    push_matrix();
    mat4.rotateX(modelview, modelview,degToRad(30));
    
    push_matrix();//#1 car
    mat4.rotate(modelview, modelview,degToRad(90+rotate_degree),[0,1,0]);
    mat4.translate(modelview,modelview,[1.8,-0.5,0]);
       
    gl.uniform4f(u_diffuseColor, 1, 0, 0, 1); // draw red to bodies of the car  COLOR!!!!

    push_matrix();//body 1
    mat4.translate(modelview,modelview,[0,1,0]);
    mat4.scale(modelview,modelview,[0.9,0.4,1.5]);
    //mat4.scale(modelview,modelview,[0.9,0.8,1.5]);
    mat4.translate(modelview,modelview,[0,-1.4,0]);
    installModel(objects[0]);
    currentModelNumber = 0;
    update_uniform(modelview, projection, 0);
    pop_matrix();//body 1
    
    push_matrix();//body 2
    mat4.translate(modelview,modelview,[0,1.5,0.3]);
    mat4.scale(modelview,modelview,[0.9,0.3,0.8]);
    mat4.translate(modelview,modelview,[0,-2.5,0]);
    installModel(objects[0]);
    currentModelNumber = 0;
    update_uniform(modelview, projection, 0);
    pop_matrix();//body 2
    
    gl.uniform4f(u_diffuseColor, 0, 0, 0, 1); // draw black to the wheel!  COLOR
    //======wheel1
    push_matrix();
    mat4.translate(modelview,modelview,[0,0.36,0.6]);
    mat4.rotate(modelview, modelview,-degToRad(90+rotate_degree*10),[1,0,0]);//rotate the wheel
    
    gl.uniform4f(u_diffuseColor, 0,0,0, 1); // draw  black brown to the wheel COLOR
    
    push_matrix();
    mat4.translate(modelview,modelview,[-1,0,0]);
    mat4.rotateY(modelview, modelview,-degToRad(90));
    mat4.translate(modelview,modelview,[0,0,-0.4]);
    installModel(objects[3]);
    currentModelNumber = 3;
    update_uniform(modelview, projection, 3);
    pop_matrix();
    
    push_matrix();
    mat4.translate(modelview,modelview,[1,0,0]);
    mat4.rotateY(modelview, modelview,-degToRad(90));
    mat4.translate(modelview,modelview,[0,0,0.4]);
    installModel(objects[3]);
    currentModelNumber = 3;
    update_uniform(modelview, projection, 3);
    pop_matrix();
    
    gl.uniform4f(u_diffuseColor, 0.87,0.72,0.53, 1); // draw  color brown to the wheel COLOR
    
    push_matrix();
    mat4.rotateY(modelview,modelview,degToRad(90));
    mat4.scale(modelview,modelview,[0.15,0.15,1.3]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
    push_matrix();// the first stick 1;  !!!!!!!!!!!!!!!!!!!!!!!!!!
    mat4.scale(modelview,modelview,[0.15,0.15,1.3]);
    mat4.translate(modelview,modelview,[-3.9,0,0]);
    mat4.scale(modelview,modelview,[1,1,0.4]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
    
    push_matrix();// the second stick 1;  
    mat4.rotateX(modelview,modelview,degToRad(120));
    mat4.scale(modelview,modelview,[0.15,0.15,1.3]);
    mat4.translate(modelview,modelview,[-3.9,0,0]);
    mat4.scale(modelview,modelview,[1,1,0.4]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
    push_matrix();// the third stick 1;  
    mat4.rotateX(modelview,modelview,degToRad(60));
    mat4.scale(modelview,modelview,[0.15,0.15,1.3]);
    mat4.translate(modelview,modelview,[-3.9,0,0]);
    mat4.scale(modelview,modelview,[1,1,0.4]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
    
    push_matrix();// the first stick 2;  !!!!!!!!!!!!!!!!!!!!!!!!!!
    mat4.scale(modelview,modelview,[0.15,0.15,1.3]);
    mat4.translate(modelview,modelview,[-3.9,0,0]);
    mat4.scale(modelview,modelview,[1,1,0.4]);
    mat4.translate(modelview,modelview,[7.85,0,0]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
    push_matrix();// the second stick 2;  
    mat4.rotateX(modelview,modelview,degToRad(120));
    mat4.scale(modelview,modelview,[0.15,0.15,1.3]);
    mat4.translate(modelview,modelview,[-3.9,0,0]);
    mat4.scale(modelview,modelview,[1,1,0.4]);
    mat4.translate(modelview,modelview,[7.85,0,0]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
    
    push_matrix();// the third stick 2;  
    mat4.rotateX(modelview,modelview,degToRad(60));
    mat4.scale(modelview,modelview,[0.15,0.15,1.3]);
    mat4.translate(modelview,modelview,[-3.9,0,0]);
    mat4.scale(modelview,modelview,[1,1,0.4]);
    mat4.translate(modelview,modelview,[7.85,0,0]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
    
    
    
    
    
    
    
    pop_matrix();
    //=======1
    
    //======wheel2
   
    
    gl.uniform4f(u_diffuseColor, 0, 0, 0, 1); // draw black to the wheel!  COLOR
    //======wheel1
    push_matrix();
    mat4.translate(modelview,modelview,[0,0.36,-0.6]);
    mat4.rotate(modelview, modelview,-degToRad(90+rotate_degree*10),[1,0,0]);//rotate the wheel
    
    gl.uniform4f(u_diffuseColor, 0,0,0, 1); // draw  black brown to the wheel COLOR
    
    push_matrix();
    mat4.translate(modelview,modelview,[-1,0,0]);
    mat4.rotateY(modelview, modelview,-degToRad(90));
    mat4.translate(modelview,modelview,[0,0,-0.4]);
    installModel(objects[3]);
    currentModelNumber = 3;
    update_uniform(modelview, projection, 3);
    pop_matrix();
    
    push_matrix();
    mat4.translate(modelview,modelview,[1,0,0]);
    mat4.rotateY(modelview, modelview,-degToRad(90));
    mat4.translate(modelview,modelview,[0,0,0.4]);
    installModel(objects[3]);
    currentModelNumber = 3;
    update_uniform(modelview, projection, 3);
    pop_matrix();
    
    gl.uniform4f(u_diffuseColor, 0.87,0.72,0.53, 1); // draw  color brown to the wheel COLOR
    
    push_matrix();
    mat4.rotateY(modelview,modelview,degToRad(90));
    mat4.scale(modelview,modelview,[0.15,0.15,1.3]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
    push_matrix();// the first stick 1;  !!!!!!!!!!!!!!!!!!!!!!!!!!
    mat4.scale(modelview,modelview,[0.15,0.15,1.3]);
    mat4.translate(modelview,modelview,[-3.9,0,0]);
    mat4.scale(modelview,modelview,[1,1,0.4]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
    
    push_matrix();// the second stick 1;  
    mat4.rotateX(modelview,modelview,degToRad(120));
    mat4.scale(modelview,modelview,[0.15,0.15,1.3]);
    mat4.translate(modelview,modelview,[-3.9,0,0]);
    mat4.scale(modelview,modelview,[1,1,0.4]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
    push_matrix();// the third stick 1;  
    mat4.rotateX(modelview,modelview,degToRad(60));
    mat4.scale(modelview,modelview,[0.15,0.15,1.3]);
    mat4.translate(modelview,modelview,[-3.9,0,0]);
    mat4.scale(modelview,modelview,[1,1,0.4]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
    
    push_matrix();// the first stick 2;  !!!!!!!!!!!!!!!!!!!!!!!!!!
    mat4.scale(modelview,modelview,[0.15,0.15,1.3]);
    mat4.translate(modelview,modelview,[-3.9,0,0]);
    mat4.scale(modelview,modelview,[1,1,0.4]);
    mat4.translate(modelview,modelview,[7.85,0,0]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
    push_matrix();// the second stick 2;  
    mat4.rotateX(modelview,modelview,degToRad(120));
    mat4.scale(modelview,modelview,[0.15,0.15,1.3]);
    mat4.translate(modelview,modelview,[-3.9,0,0]);
    mat4.scale(modelview,modelview,[1,1,0.4]);
    mat4.translate(modelview,modelview,[7.85,0,0]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
    
    push_matrix();// the third stick 2;  
    mat4.rotateX(modelview,modelview,degToRad(60));
    mat4.scale(modelview,modelview,[0.15,0.15,1.3]);
    mat4.translate(modelview,modelview,[-3.9,0,0]);
    mat4.scale(modelview,modelview,[1,1,0.4]);
    mat4.translate(modelview,modelview,[7.85,0,0]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
    
    
    
    
    
    
    
    pop_matrix();
    //=======2
    
    //=====car light
    push_matrix();// the first light
    if (!is_sun)
        gl.uniform4f(u_diffuseColor, 1, 1, 0,1);
    else
        gl.uniform4f(u_diffuseColor, 1, 1, 0.3,1);
        
    installModel(objects[2]);
    mat4.scale(modelview,modelview,[0.3,0.3,0.3]);
    mat4.translate(modelview, modelview, [0.5, 1.5, -2.3]);
    var temp = [0, 0, 0, 0];
    vec4.transformMat4(temp, [0, 0, 0, 1], modelview);
    gl.uniform4f(u_carpos1, temp[0], temp[1], temp[2], 1);
    push_matrix();
    mat4.translate(modelview, modelview, [0, 0, -2]);
    vec4.transformMat4(temp, [0, 0, 0, 1], modelview);
    gl.uniform4f(u_cardir, temp[0], temp[1], temp[2], 1);
    pop_matrix();
    currentModelNumber = 2;
    update_uniform(modelview, projection, 2);
    pop_matrix();
    
    
    push_matrix();// the second light
    if (!is_sun)
        gl.uniform4f(u_diffuseColor, 1, 1, 0,1);
    else
        gl.uniform4f(u_diffuseColor, 1, 1, 0.3,1);
    installModel(objects[2]);
    mat4.scale(modelview,modelview,[0.3,0.3,0.3]);
    mat4.translate(modelview, modelview, [-0.7, 1.5, -2.3]);
    vec4.transformMat4(temp, [0, 0, 0, 1], modelview);
    gl.uniform4f(u_carpos2, temp[0], temp[1], temp[2], 1);
    currentModelNumber = 2;
    update_uniform(modelview, projection, 2);
    pop_matrix();
    
    //=====
    
    pop_matrix();//car   POP the CAR environment!
    
   
    push_matrix();//ring
    gl.uniform4f(u_diffuseColor, 1,1,1, 1); // draw  color white to the ring COLOR
    mat4.translate(modelview,modelview,[0,-0.5,0]);
    mat4.scale(modelview,modelview,[5,5,5]);
    mat4.rotateX(modelview,modelview,-degToRad(90));
    installModel(objects[1]);
    currentModelNumber = 1;
    update_uniform(modelview, projection, 1);
    pop_matrix();//ring
    
    
    //===grass ground
    push_matrix();
    gl.uniform4f(u_diffuseColor, 0.5,1,0, 1); // draw  color green to the grassground
    mat4.translate(modelview,modelview,[-2,1,0]);
    mat4.scale(modelview,modelview,[7.5,0.3,7.5]);
    mat4.rotateX(modelview,modelview,-degToRad(90));
    mat4.translate(modelview,modelview,[0.27,0,-5.58]);
    installModel(objects[4]);
    currentModelNumber = 4;
    update_uniform(modelview, projection, 4);
    pop_matrix();// pop grass ground
    
    //====sun environment
    push_matrix();
    if(is_sun)
        gl.uniform4f(u_diffuseColor,1,1,0, 1); // draw yellow to the sun
    else
        gl.uniform4f(u_diffuseColor,0.3843, 0.3098, 0.3098, 1); // draw  dark grey to the sun
    mat4.rotateZ(modelview,modelview,degToRad(90+rotate_degree));
    mat4.scale(modelview,modelview,[0.7,0.7,0.7]);
    mat4.translate(modelview,modelview,[-0.9,6,0]);
    //mat4.translate(modelview,modelview,[0,-6,0]);
    installModel(objects[2]);
    currentModelNumber = 2;
    update_uniform(modelview, projection, 2);
    
    if(is_sun)
    {
        
        var light_position = [
        0,0,0,1
    ];
        
    vec4.transformMat4(light_position, light_position, modelview);
    gl.uniform4fv(u_lightPosition, light_position);
    }
    pop_matrix();

    //==pop sun environment
    
    
    //==load tree
    
    push_matrix();// tree in the center
    mat4.scale(modelview,modelview,[0.7,0.7,0.7]);
    mat4.translate(modelview,modelview,[0,-0.2,1.2]);
    draw_tree();
    pop_matrix();
    
    push_matrix()// tree in the center
    mat4.scale(modelview,modelview,[0.7,0.7,0.7]);
    mat4.translate(modelview,modelview,[0.6,-0.2,-0.8]);
    draw_tree();
    pop_matrix();
    
    push_matrix()// tree in the center
    mat4.scale(modelview,modelview,[0.4,0.4,0.4]);
    mat4.translate(modelview,modelview,[-1.9,-0.8,-0.1]);
    draw_tree();
    pop_matrix();
    
    
    push_matrix()// tree in the outside of the ring
    mat4.scale(modelview,modelview,[0.5,0.5,0.5]);
    mat4.translate(modelview,modelview,[-4,-0.5,-4.3]);
    draw_tree();
    pop_matrix();
    
    
    push_matrix()// tree in the outside of the ring
    mat4.scale(modelview,modelview,[0.5,0.5,0.5]);
    mat4.translate(modelview,modelview,[6.5,-0.5,0]);
    draw_tree();
    pop_matrix();
    
    push_matrix()// tree in the outside of the ring
    mat4.scale(modelview,modelview,[0.4,0.4,0.4]);
    mat4.translate(modelview,modelview,[2,-0.8,-7.5]);
    draw_tree();
    pop_matrix();
    
    push_matrix()// tree in the outside of the ring
    mat4.scale(modelview,modelview,[0.35,0.35,0.35]);
    mat4.translate(modelview,modelview,[9,-1,-2]);
    draw_tree();
    pop_matrix();
    
    
    push_matrix()// tree in the outside of the ring
    mat4.scale(modelview,modelview,[0.25,0.25,0.25]);
    mat4.translate(modelview,modelview,[12,-1.5,2]);
    draw_tree();
    pop_matrix();
    
    push_matrix()// tree in the outside of the ring
    mat4.scale(modelview,modelview,[0.25,0.25,0.25]);
    mat4.translate(modelview,modelview,[5,-1.5,-11]);
    draw_tree();
    pop_matrix();
    
    push_matrix()// tree in the outside of the ring
    mat4.scale(modelview,modelview,[0.25,0.25,0.25]);
    mat4.translate(modelview,modelview,[0,-1.5,13]);
    draw_tree();
    pop_matrix();
    
    
    //==  done with loading tree
    
    
    
    
    
    //== draw  road light
    //== cylinder first
    push_matrix();
    gl.uniform4f(u_diffuseColor, 0.752,0.752,0.752, 1); // draw  silver to the cylinder of the road light
    installModel(objects[4]);
    currentModelNumber = 4;
    mat4.rotateX(modelview,modelview,-degToRad(90));
    mat4.scale(modelview,modelview,[0.15,0.15,1.6]);
    update_uniform(modelview, projection, 4);
    pop_matrix();
    // done with cylinder
    
    //==ball light second
    push_matrix();
    if(is_sun)
        gl.uniform4f(u_diffuseColor,0.3843, 0.3098, 0.3098, 1); // draw  dark grey to the ball light
    else
        gl.uniform4f(u_diffuseColor,1, 1, 0, 1); 
    installModel(objects[2]);
    currentModelNumber = 2;
    mat4.scale(modelview,modelview,[0.35,0.35,0.35]);
    mat4.translate(modelview,modelview,[0,2,0]);
    update_uniform(modelview, projection, 2);
    if(!is_sun)
        {
            var light_position = [
        0,0,0,1
    ];
    vec4.transformMat4(light_position, light_position, modelview);
    gl.uniform4fv(u_lightPosition, light_position);
        }
    pop_matrix();
    
    //==done with ball light
    
    
    //done with road light
    
    
    
    
    pop_matrix();

    

    
    
    
    
    
    
    
    
    

   


   
   

    


   

}

// a function to draw the tree
function draw_tree(){
    push_matrix();
    gl.uniform4f(u_diffuseColor, 0.5,1,0, 1); // draw green to the tree  to the leaves
    installModel(objects[5]);
    currentModelNumber = 5;
    mat4.rotateX(modelview,modelview,-degToRad(90));
    mat4.translate(modelview,modelview,[0,0,0.8]);
    update_uniform(modelview, projection, 5);
    pop_matrix(); 
    
    
    push_matrix();
    gl.uniform4f(u_diffuseColor, 0.545, 0.27, 0.0745, 1); // draw brown to the root of the tree
    installModel(objects[4]);
    currentModelNumber = 4;
    mat4.rotateX(modelview,modelview,-degToRad(90));
    mat4.scale(modelview,modelview,[0.7,0.7,0.7]);
    mat4.translate(modelview,modelview,[0,0,-0.25]);
    update_uniform(modelview, projection, 4);
    pop_matrix();
    
   
    
    
}

/*
  this function assigns the computed values to the uniforms for the model, view and projection 
  transform
*/
function update_uniform(modelview,projection,currentModelNumber){

    /* Get the matrix for transforming normal vectors from the modelview matrix,
       and send matrices to the shader program*/
    mat3.normalFromMat4(normalMatrix, modelview);
    
    gl.uniformMatrix3fv(u_normalMatrix, false, normalMatrix);
    gl.uniformMatrix4fv(u_modelview, false, modelview );
    gl.uniformMatrix4fv(u_projection, false, projection );   
    gl.drawElements(gl.TRIANGLES, objects[currentModelNumber].indices.length, gl.UNSIGNED_SHORT, 0);
}



/* 
 * Called and data for the model are copied into the appropriate buffers, and the 
 * scene is drawn.
 */
function installModel(modelData) {
     gl.bindBuffer(gl.ARRAY_BUFFER, a_coords_buffer);
     gl.bufferData(gl.ARRAY_BUFFER, modelData.vertexPositions, gl.STATIC_DRAW);
     gl.vertexAttribPointer(a_coords_loc, 3, gl.FLOAT, false, 0, 0);
     gl.enableVertexAttribArray(a_coords_loc);
     gl.bindBuffer(gl.ARRAY_BUFFER, a_normal_buffer);
     gl.bufferData(gl.ARRAY_BUFFER, modelData.vertexNormals, gl.STATIC_DRAW);
     gl.vertexAttribPointer(a_normal_loc, 3, gl.FLOAT, false, 0, 0);
     gl.enableVertexAttribArray(a_normal_loc);
     gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,index_buffer);
     gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, modelData.indices, gl.STATIC_DRAW);
}


/* Initialize the WebGL context.  Called from init() */
function initGL() {
    var prog = createProgram(gl,"vshader-source","fshader-source");
    gl.useProgram(prog);
    a_coords_loc =  gl.getAttribLocation(prog, "a_coords");
    a_normal_loc =  gl.getAttribLocation(prog, "a_normal");
    u_modelview = gl.getUniformLocation(prog, "modelview");
    u_projection = gl.getUniformLocation(prog, "projection");
    u_normalMatrix =  gl.getUniformLocation(prog, "normalMatrix");
    u_lightPosition=  gl.getUniformLocation(prog, "lightPosition");
    u_diffuseColor =  gl.getUniformLocation(prog, "diffuseColor");
    u_specularColor =  gl.getUniformLocation(prog, "specularColor");
    u_specularExponent = gl.getUniformLocation(prog, "specularExponent");
    u_carpos1 = gl.getUniformLocation(prog, "carpos1");
    u_carpos2 = gl.getUniformLocation(prog, "carpos2");
    u_cardir = gl.getUniformLocation(prog, "cardir");
    u_is_sun = gl.getUniformLocation(prog, "is_sun");

    a_coords_buffer = gl.createBuffer();
    a_normal_buffer = gl.createBuffer();
    index_buffer = gl.createBuffer();
    gl.enable(gl.DEPTH_TEST);
    gl.uniform3f(u_specularColor, 0.5, 0.5, 0.5);
    gl.uniform4f(u_diffuseColor, 1, 1, 1, 1);
    gl.uniform1f(u_specularExponent, 10);
    gl.uniform4f(u_lightPosition, 0, 0, 0, 1);
}

/* Creates a program for use in the WebGL context gl, and returns the
 * identifier for that program.  If an error occurs while compiling or
 * linking the program, an exception of type String is thrown.  The error
 * string contains the compilation or linking error.  If no error occurs,
 * the program identifier is the return value of the function.
 *    The second and third parameters are the id attributes for <script>
 * elementst that contain the source code for the vertex and fragment
 * shaders.
 */
function createProgram(gl, vertexShaderID, fragmentShaderID) {
    function getTextContent( elementID ) {
            // This nested function retrieves the text content of an
            // element on the web page.  It is used here to get the shader
            // source code from the script elements that contain it.
        var element = document.getElementById(elementID);
        var node = element.firstChild;
        var str = "";
        while (node) {
            if (node.nodeType == 3) // this is a text node
                str += node.textContent;
            node = node.nextSibling;
        }
        return str;
    }
    try {
        var vertexShaderSource = getTextContent( vertexShaderID );
        var fragmentShaderSource = getTextContent( fragmentShaderID );
    }
    catch (e) {
        throw "Error: Could not get shader source code from script elements.";
    }
    var vsh = gl.createShader( gl.VERTEX_SHADER );
    gl.shaderSource(vsh,vertexShaderSource);
    gl.compileShader(vsh);
    if ( ! gl.getShaderParameter(vsh, gl.COMPILE_STATUS) ) {
        throw "Error in vertex shader:  " + gl.getShaderInfoLog(vsh);
     }
    var fsh = gl.createShader( gl.FRAGMENT_SHADER );
    gl.shaderSource(fsh, fragmentShaderSource);
    gl.compileShader(fsh);
    if ( ! gl.getShaderParameter(fsh, gl.COMPILE_STATUS) ) {
       throw "Error in fragment shader:  " + gl.getShaderInfoLog(fsh);
    }
    var prog = gl.createProgram();
    gl.attachShader(prog,vsh);
    gl.attachShader(prog, fsh);
    gl.linkProgram(prog);
    if ( ! gl.getProgramParameter( prog, gl.LINK_STATUS) ) {
       throw "Link error in program:  " + gl.getProgramInfoLog(prog);
    }
    return prog;
}


/**
 * initialization function that will be called when the page has loaded
 */
function init() {
    try {
        var canvas = document.getElementById("myGLCanvas");
        gl = canvas.getContext("webgl") || 
                         canvas.getContext("experimental-webgl");
        if ( ! gl ) {
            throw "Browser does not support WebGL";
        }
    }
    catch (e) {
        document.getElementById("canvas-holder").innerHTML =
            "<p>Sorry, could not get a WebGL graphics context.</p>";
        return;
    }

    try {
        initGL();  // initialize the WebGL graphics context
    }
    catch (e) {
        document.getElementById("canvas-holder").innerHTML =
            "<p>Sorry, could not initialize the WebGL graphics context:" + e + "</p>";
        return;
    }

    document.getElementById("animation").checked = false;
    document.getElementById("animation").onchange = draw;
    rotator = new TrackballRotator(canvas, draw, 15);
    tick();
}

function tick(){
     requestAnimFrame(tick);
     draw();
}








var scene, renderer;

// The different levels on the mobile
var ball;

var topCamera, perspectiveCamera, ballCamera, currentCamera;

var cue1, cue2, cue3, cue4, cue5, cue6;
var cues = [];
var currentCue = null;

var rotateCCW = false;
var rotateCW = false;

var throwWhite = false;

var changeCue = false;
var cue = 0;
var previousCue = 0;

// Flag to changeCameraEvent
var changeToCamera;

// Array to keep the materials in order to change their wireframe
var materials = [];

// List of initial balls and walls
var balls = []
var walls = []

var whiteBall = null;
var trackingBall = null;

var holes = [new THREE.Vector3(48, 2, 88),
             new THREE.Vector3(-48, 2, 88),
             new THREE.Vector3(52, 2, 0),
             new THREE.Vector3(-52, 2, 0),
             new THREE.Vector3(48, 2, -88),
             new THREE.Vector3(-48, 2, -88)];

var clock;

const FACTOR = 3.5;

function createBall(x, y, z, color) {
    'use strict';
    var ball = new THREE.Object3D();
    ball.data = {velocity: 0, angle: 0, goingDown: false};

    var material = new THREE.MeshBasicMaterial({color: color, wireframe: true});
    var geometry = new THREE.SphereGeometry(2, 32, 32);
    var mesh = new THREE.Mesh(geometry, material);

    ball.add(mesh);
    ball.position.set(x, y, z)

    scene.add(ball);
    return ball;
}

function giveMovement(ball, angle, velocity) {
    'use strict'
    ball.data.velocity = velocity;
    ball.data.angle = angle;
}

function createCue(x, z, offSetX, offSetZ, rotX, rotY) {
    'use strict';
    var taco = new THREE.Object3D()   
    taco.data = {initialAngle: rotY, previousAngle: rotY, currentAngle: rotY};

    var material = new THREE.MeshBasicMaterial({ color: 0xffffff, wireframe: true });
    var geometry = new THREE.CylinderGeometry(1, 1.5, 80, 32);
    geometry.rotateX(rotX).rotateY(rotY);
    geometry.translate(offSetX, 0, offSetZ);

    var mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(offSetX, 0, offSetZ);
    materials.push(material);  

    taco.translateX(x).translateZ(z);
    
    taco.add(mesh);
    scene.add(taco);
    return taco;
}

function createCube(obj, x, y, z, l, c, h, rotX, rotY, rotZ, color) {
    'use strict';

    var material = new THREE.MeshBasicMaterial({ color: color, wireframe: false });
    var geometry = new THREE.BoxGeometry(l, c, h);
    geometry.rotateX(rotX).rotateY(rotY).rotateZ(rotZ);

    var mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);
}

function createBox(x, y, z, l, c, h, color) {
    'use strict';
    var box = new THREE.Box3();
    var material = new THREE.MeshBasicMaterial({ color: color, wireframe: false });
    var geometry = new THREE.BoxGeometry(l, c, h);

    var mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);

    mesh.geometry.computeBoundingBox();

    box.copy(mesh.geometry.boundingBox).applyMatrix4(mesh.matrixWorld);
    scene.add(mesh);
    walls.push(box);
}

function createShape() {
    'use strict';
    var corners = [
        new THREE.Vector2(58, 98),
        new THREE.Vector2(-58, 98),
        new THREE.Vector2(-58, -98),
        new THREE.Vector2(58, -98),
    ];
    var shape = new THREE.Shape(corners);
    
    var holeBottomRight = new THREE.Path();
    holeBottomRight.absarc(48, 88, 8, 0, Math.PI * 2, true);
    shape.holes.push(holeBottomRight);

    var holeBottomLeft = new THREE.Path();
    holeBottomLeft.absarc(-48, 88, 8, 0, Math.PI * 2, true);
    shape.holes.push(holeBottomLeft);

    var holeMiddleRight = new THREE.Path();
    holeMiddleRight.absarc(52, 0, 8.5, -Math.PI/2, Math.PI/2, true);
    shape.holes.push(holeMiddleRight);

    var holeMiddleLeft = new THREE.Path();
    holeMiddleLeft.absarc(-52, 0, 8.5, Math.PI/2, -Math.PI/2, true);
    shape.holes.push(holeMiddleLeft);

    var holeTopLeft = new THREE.Path();
    holeTopLeft.absarc(48, -88, 8, 0, Math.PI * 2, true);
    shape.holes.push(holeTopLeft);

    var holeTopRight = new THREE.Path();
    holeTopRight.absarc(-48, -88, 8, 0, Math.PI * 2, true);
    shape.holes.push(holeTopRight);    

    var geometry = new THREE.ShapeBufferGeometry(shape);
    geometry.rotateX(Math.PI / 2);
    var material = new THREE.MeshBasicMaterial({ color: 0x00b300, wireframe: false, side: THREE.DoubleSide });
    var mesh = new THREE.Mesh(geometry, material);
    scene.add(mesh);
}

function createTable() {
    'use strict';
    var table = new THREE.Object3D()
    // Base
    createShape();
    // Lado direito
    createCube(table, 54, 4, 0, 8, 16, 195, 0, 0, 0, 0x654321);
    // Lado esquerdo
    createCube(table, -54, 4, 0, 8, 16, 195, 0, 0, 0, 0x654321);
    // Fundo
    createCube(table, 0, 4, -94, 115, 16, 8, 0, 0, 0, 0x654321);
    // Frente
    createCube(table, 0, 4, 94, 115, 16, 8, 0, 0, 0, 0x654321);
    scene.add(table);
}

// Creates 15 initial balls
function createInitialBalls() {
    'use strict';
    var colors = [0xff0000, 0x00fff0, 0x0000ff, 0xe05a00, 0x800080, 0xffff00];
    for (let i = 0; i < 15; i++) {

        var x = ((i % 3) - 1) * 30;
        var z = (Math.floor(i / 3) - 2) * 30;

        //Random angle
        var angle = Math.floor(Math.random()*(Math.PI*2));

        // Random velocity
        var vel = Math.floor(Math.random()*(150 - 100) + 100);
        ball = createBall(x, 2, z, colors[i % colors.length]);
        giveMovement(ball, angle, vel);
        balls.push(ball);
    }
}

function createScene() {
    'use strict';
    scene = new THREE.Scene();
    createTable();
    createInitialBalls();
}

function createCameras() {
    'use strict';
    currentCamera = changeToCamera = topCamera = createTopCamera(topCamera, 0, 300, 0);
    perspectiveCamera = createPerspectiveCamera(perspectiveCamera, 120, 120, 120);
    ballCamera = null;
}

function createTopCamera(cameraToCreate, x, y, z) {
    'use strict';
    cameraToCreate = new THREE.OrthographicCamera(window.innerWidth / -FACTOR, window.innerWidth / FACTOR, window.innerHeight / FACTOR, window.innerHeight / -FACTOR, 1, 1000 );
    cameraToCreate.position.x = x;
    cameraToCreate.position.y = y;
    cameraToCreate.position.z = z;
    cameraToCreate.lookAt(scene.position);
    return cameraToCreate;
}

function createPerspectiveCamera(cameraToCreate, x, y, z) {
    'use strict';
    var camera = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight, 1,1000);
    camera.position.x = x;
    camera.position.y = y;
    camera.position.z = z;
    camera.lookAt(scene.position);
    return camera;
}

function setBallCam(cameraToSet) {
    'use strict'
    cameraToSet.position.x = trackingBall.position.x;
    cameraToSet.position.y = 50;
    cameraToSet.position.z = trackingBall.position.z + 50;
    cameraToSet.lookAt(trackingBall.position);
}

function onResize() {
    'use strict';
    renderer.setSize(window.innerWidth, window.innerHeight);
    
    if (window.innerHeight > 0 && window.innerWidth > 0) {
        if (currentCamera === topCamera) {
            currentCamera.left = window.innerWidth / -FACTOR;
            currentCamera.right = window.innerWidth / FACTOR;
            currentCamera.top = window.innerHeight / FACTOR;
            currentCamera.bottom = window.innerHeight / -FACTOR;
        } else {        
            currentCamera.aspect = window.innerWidth / window.innerHeight;
        }
        currentCamera.updateProjectionMatrix();
    }
}

function onKeyDown(e) {
    'use strict';

    switch (e.keyCode) {
        // 1, Select Front Camera
        case 49: {
            changeToCamera = topCamera;
            break;
        }
        // 2, Select Top Camera
        case 50: {
            changeToCamera = perspectiveCamera;
            break;
        }
        // 3, Select Side Camera
        case 51: {
            if (trackingBall)
                changeToCamera = ballCamera;
            break;
        }
        // 4, Select cue 1
        case 52: {
            changeCue = true;
            cue = 1;
            break;
        }
        // 5, Select cue 2
        case 53: {
            changeCue = true;
            cue = 2;
            break;
        }
        // 6, Select cue 3
        case 54: {
            changeCue = true;
            cue = 3;
            break;
        }
        // 7, Select cue 4
        case 55: {
            changeCue = true;
            cue = 4;
            break;
        }
        // 8, Select cue 5
        case 56: {
            changeCue = true;
            cue = 5;
            break;
        }
        // 9, Select cue 6
        case 57: {
            changeCue = true;
            cue = 6;
            break;
        }
        // Left Arrow
        case 37: {
            rotateCW = true;
            break;
        }
        // Right Arrow
        case 39: {
            rotateCCW = true;
            break;
        }
        // White Space
        case 32: {
            throwWhite = true;
            break;
        }
    }
}

function render() {
    'use strict';
    renderer.render(scene, currentCamera);
}

function createCues() {
    'use strict'
    cues = [];
    materials = [];
    cue1 = createCue(-40, -45, -40, 0, Math.PI/2, Math.PI/2);
    cue2 = createCue(-40, 45, -40, 0, Math.PI/2, Math.PI/2);
    cue3 = createCue(0, 80, 0, 40, Math.PI/2, Math.PI);
    cue4 = createCue(40, 45, 40, 0, Math.PI/2, -Math.PI/2);
    cue5 = createCue(40, -45, 40, 0, Math.PI/2, -Math.PI/2);
    cue6 = createCue(0, -80, 0, -40, Math.PI/2, 0);
    cues.push(cue1, cue2, cue3, cue4, cue5, cue6);
}

function moveBalls(step) {
    'use strict'
    balls.forEach(ball => {
        if (!ball.data.goingDown) {
            ball.position.x += step * ball.data.velocity * Math.cos(ball.data.angle)
            ball.position.z += step * ball.data.velocity * Math.sin(ball.data.angle)

            if (ball === trackingBall) {
                ballCamera.position.x += step * ball.data.velocity * Math.cos(ball.data.angle) * 1.5;
                ballCamera.position.z += step * ball.data.velocity * Math.sin(ball.data.angle) * 1.5;
                ballCamera.lookAt(trackingBall.position);
            }

            // Handle with rotations
            if (ball.data.velocity > 0) { 
                var axis = new THREE.Vector3();
                axis.set(step * ball.data.velocity * Math.cos(ball.data.angle), 0, step * ball.data.velocity * Math.sin(ball.data.angle)).normalize();
                axis.cross(THREE.Object3D.DefaultUp);

                const angle = -0.5 / (Math.PI * 2) * Math.PI;
                ball.rotateOnAxis(axis, angle);
            }

        } else {
            ball.position.y -= step * 100;
            if (ball === trackingBall) {
                ballCamera.position.y -= step * 100;
            }
        }
    });
}

function updateSpeeds(step) {
    'use strict'
    balls.forEach(ball => {
        if (!ball.data.goingDown) {
            if (ball.data.velocity > 0) {
                ball.data.velocity -= 10 * step;
            }
            if (ball.data.velocity < 0) {
                ball.data.velocity = 0;
            }
        }
    });
}

function positiveAngle(angle) {
    'use strict'
    if (angle > 2 * Math.PI) {
        return angle - 2 * Math.PI;
    } 
    if (angle < 0) {
        return angle + 2 * Math.PI;
    }
    return angle;
}

function detectAndHandleBallWallsCollisions(step) {
    'use strict'
    balls.forEach(ball => {
        var angle = ball.data.angle;
        
        if (ball.position.x >= 48) {
            // Right
            ballsUncollide(ball, step);
            if (angle <= Math.PI) {
                ball.data.angle = positiveAngle(Math.PI - angle);
            } else {
                ball.data.angle = positiveAngle(-Math.PI/2 - (angle - (3/2)*Math.PI));
            }
        } else if (ball.position.x <= -48) {
            // Left
            ballsUncollide(ball, step);
            if (angle >= Math.PI) {
                ball.data.angle = positiveAngle(2 * (3/2)*Math.PI - angle);
            } else {
                ball.data.angle = positiveAngle(Math.PI - angle);
            }
        }
        if (ball.position.z >= 88) {
            // Bottom
            ballsUncollide(ball, step);
            if (angle >= Math.PI/2) {
                ball.data.angle = positiveAngle(2*Math.PI - angle);
            } else {
                ball.data.angle = positiveAngle(-angle);
            }
        } else if (ball.position.z <= -88) {
            // Top
            ballsUncollide(ball, step);
            if (angle >= Math.PI/2) {
                ball.data.angle = positiveAngle(-angle); 
            } else {
                ball.data.angle = positiveAngle(2*Math.PI - angle); 
            }
        }
    });
}

function checkCollisions(pos1, pos2) {
    'use strict'
    return 16 >= Math.pow(pos1.x - pos2.x, 2) + Math.pow(pos1.z - pos2.z, 2);
}

function ballsUncollide(ball, step) {
    'use strict'
    ball.position.x -= step * 0.95 * ball.data.velocity * Math.cos(ball.data.angle);
    ball.position.z -= step * 0.95 * ball.data.velocity * Math.sin(ball.data.angle);
}

function calculatePhi(pos1, pos2) {
    'use strict'
    var vector = [pos1.x - pos2.x, pos1.z - pos2.z];
    var angle;
    if (vector[0] == 0) {
        angle =  Math.PI/2;
    } else {        
        angle = Math.atan(vector[1] / vector[0]);
    }
    return angle;
}

function findAngle(vx, vz) {
    'use strict'
    var angle;
    // Correcting and calculating the final angles after the collisions
    if (vx < 0) {
        angle = Math.PI + Math.atan(vz/vx);
    } else if (vx > 0 && vz >= 0) {
        angle = Math.atan(vz/vx);
    } else if (vx > 0 && vz < 0) {
        angle = Math.PI * 2 + Math.atan(vz/vx);
    } else if (vx === 0 && vz === 0) {
        angle = 0;
    } else if (vx === 0 && vz >= 0) {
        angle =  Math.PI / 2;
    } else {
        angle =  3 * Math.PI / 2;
    }
    return angle;
}

function findVelocities(b1, b2) {
    'use strict'
    // Elastic collision formula
    var phi = calculatePhi(b1.position, b2.position);
    var v1x = b2.data.velocity * Math.cos(b2.data.angle - phi) * Math.cos(phi) + b1.data.velocity * Math.sin(b1.data.angle - phi) * Math.cos(phi + Math.PI/2);
    var v1z = b2.data.velocity * Math.cos(b2.data.angle - phi) * Math.sin(phi) + b1.data.velocity * Math.sin(b1.data.angle - phi) * Math.sin(phi + Math.PI/2);
    return [v1x, v1z];
}

function handleCollisions(collisions) {
    'use strict'
    balls.forEach(ball => {
        var v1XTotal = 0;
        var v1ZTotal = 0;
        var hasCollision = false;
        collisions.forEach(collision => {
            if (collision[0] === ball) {
                v1XTotal += collision[1];
                v1ZTotal += collision[2];
                hasCollision = true;
            }
        });
        // If a collision happens, sum all directed velocities and calculate the final one.
        // Calculating the angle with the final velocity 
        if (hasCollision) {
            var theta = findAngle(v1XTotal, v1ZTotal);
            var vTotal = Math.sqrt(Math.pow(v1XTotal, 2) + Math.pow(v1ZTotal, 2));
            ball.data.velocity = vTotal;
            ball.data.angle = theta;
        }
    });
}

function detectAndHandleBallBallCollisions(step) {
    'use strict'
    // Array to store multiple ball collisions
    var collisions = [];
    for (let i = 0; i < balls.length; i++) {
        var b1 = balls[i];
        for (let j = i + 1; j < balls.length; j++) {
            var b2 = balls[j];
            if (checkCollisions(b1.position, b2.position) && !b1.data.goingDown && !b2.data.goingDown) {
                ballsUncollide(b1, step);
                ballsUncollide(b2, step);
                var b1Res = findVelocities(b1, b2);
                var b2Res = findVelocities(b2, b1);
                collisions.push([b1, b1Res[0], b1Res[1]]);
                collisions.push([b2, b2Res[0], b2Res[1]]);
            }
        }
    }
    handleCollisions(collisions);
}

function detectBallInHole() {
    'use strict'
    balls.forEach(ball => {
        holes.forEach(hole => {
            var distance = ball.position.distanceTo(hole);
            if (distance < 6) {
                ball.data.goingDown = true;
            }
        });
    });
}

function deleteBalls() {
    'use strict'
    var removedBalls = []
    balls.forEach(ball => {
        if (ball.position.y < -500) {
            scene.remove(ball);
            
            if (ball === trackingBall) {
                changeToCamera = topCamera;
                trackingBall = null;
            }
            removedBalls.push(ball);
        }
    });
    removedBalls.forEach(removed => {
        balls.splice(balls.indexOf(removed), 1);
    });
}

function changeColorCue(cue) {
    'use strict'
    for (let i = 0; i < materials.length; i++) {
        if (cue - 1 === i) {
            materials[i].color.setHex(0xffff00);
        } else {
            materials[i].color.setHex(0xffffff);
        }
    }
}

function deleteCues() {
    'use strict'
    cues.forEach(cue => {
        scene.remove(cue);
    });
}

function changeCues() {
    'use strict'
    if (changeCue) {
        deleteCues();
        createCues();
        if (whiteBall !== null) {
            scene.remove(whiteBall);
        }
        if (cue === previousCue) {
            cue = 0;
            previousCue = 0;
            currentCue = trackingBall = null;

            if (currentCamera == ballCamera) {
                changeToCamera = topCamera;
            }
            return;
        }
        previousCue = cue;

        if (cue === 1) {
            currentCue = cue1;
            changeColorCue(1);
            whiteBall = createBall(-40, 2, -45, 0xffffff);
            whiteBall.data.angle = Math.PI/2;
            changeCue = false;
        }
        else if (cue === 2) {
            currentCue = cue2;
            changeColorCue(2);
            whiteBall = createBall(-40, 2, 45, 0xffffff);
            whiteBall.data.angle = Math.PI/2;
            changeCue = false;
        }
        else if (cue === 3) {
            currentCue = cue3;
            changeColorCue(3);
            whiteBall = createBall(0, 2, 80, 0xffffff);
            whiteBall.data.angle = 0;
            changeCue = false;
        }
        else if (cue === 4) {
            currentCue = cue4;
            changeColorCue(4);
            whiteBall = createBall(40, 2, 45, 0xffffff);
            whiteBall.data.angle = -Math.PI/2
            changeCue = false;
        }
        else if (cue === 5) {
            currentCue = cue5;
            changeColorCue(5);
            whiteBall = createBall(40, 2, -45, 0xffffff);
            whiteBall.data.angle = -Math.PI/2
            changeCue = false;
        }
        else if (cue === 6) {
            currentCue = cue6;
            changeColorCue(6);
            whiteBall = createBall(0, 2, -80, 0xffffff);
            whiteBall.data.angle = Math.PI
            changeCue = false;
        }
        trackingBall = whiteBall;
        scene.add(whiteBall);
        setBallCam(ballCamera);
    }
}

function rotateCue(step) {
    'use strict'
    if (rotateCCW || rotateCW) {
        if (currentCue !== null) {
            if (rotateCW) {
                currentCue.data.currentAngle = currentCue.data.previousAngle - step; 

                if (currentCue.data.currentAngle - currentCue.data.initialAngle < -Math.PI/4) {
                    currentCue.data.currentAngle = currentCue.data.initialAngle - Math.PI/4;
                }
                                
                currentCue.rotateY(currentCue.data.currentAngle - currentCue.data.previousAngle);
                whiteBall.data.angle -= currentCue.data.currentAngle - currentCue.data.previousAngle;
                currentCue.data.previousAngle = currentCue.data.currentAngle;
            } else {
                currentCue.data.currentAngle = currentCue.data.previousAngle + step;

                if (currentCue.data.currentAngle - currentCue.data.initialAngle > Math.PI/4) {
                    currentCue.data.currentAngle = currentCue.data.initialAngle + Math.PI/4;
                }

                currentCue.rotateY(currentCue.data.currentAngle - currentCue.data.previousAngle);
                whiteBall.data.angle -= currentCue.data.currentAngle - currentCue.data.previousAngle;
                currentCue.data.previousAngle = currentCue.data.currentAngle;
            }
        }
        rotateCCW = rotateCW = false;
    }
}

function moveWhiteBall() {
    'use strict';
    if (throwWhite) {
        if (whiteBall !== null) {
            giveMovement(whiteBall, whiteBall.data.angle - Math.PI/2, 100);
            currentCue = null;
            balls.push(whiteBall);
            whiteBall = null;
            deleteCues();
            createCues();
        }
        throwWhite = false;
    }
}

function init() {
    'use strict';
    renderer = new THREE.WebGLRenderer({
        antialias: true
    });
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);

    createScene();
    createCues();
    createCameras();

    clock = new THREE.Clock();
    ballCamera = createPerspectiveCamera(ballCamera, 0, 0, 0)

    window.addEventListener("keydown", onKeyDown);
    window.addEventListener("resize", onResize);
}

function animate() {
    'use strict';
    var step = clock.getDelta();
    if (changeToCamera !== currentCamera) {
        currentCamera = changeToCamera
        onResize();
    }

    moveBalls(step);
    changeCues();
    rotateCue(step);
    moveWhiteBall();
    updateSpeeds(step);
    deleteBalls();
    detectAndHandleBallWallsCollisions(step);
    detectBallInHole();
    detectAndHandleBallBallCollisions(step);
    render();
    requestAnimationFrame(animate);
}

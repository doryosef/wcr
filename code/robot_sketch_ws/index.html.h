const char INDEX_HTML[] = R"======( 
HTTP/1.1 200 OK
Content-type:text/html

<html lang="en" xmlns="http://www.w3.org/1999/html">
   <head>
      <title>touch event example</title>
      <meta charset="utf-8"/>
   </head>
   <body>
     <div style="text-align:center; margin-top:10%">
      <canvas id="canvas" >
         Your browser does not support canvas element.
      </canvas>
    </div>
      <br>
      <br>
   </body>
   <script>
   
    // esp32 ip route
    let ws = new WebSocket("ws://LOCAL_IP:8080"), 
        canvas = document.getElementById("canvas"),
        middleCircle = {},
    frameCircle = {},
    isWSactive = false,
        ctx = canvas.getContext("2d");
  
  initCanvas();
  initFrameCircle();
    initMiddleCircle();
    renderScene();

  function initCanvas() {
    canvas.width = window.innerWidth * 0.7;
    canvas.height = window.innerWidth * 0.7;
  }
  
  function initFrameCircle() {
    ctx.beginPath();
    frameCircle.x = canvas.width/2;
    frameCircle.y = canvas.height/2;
    // divide canvas width + height by 2 * 100 for percentage radius is only 50%
    frameCircle.radius = ( ((canvas.width + canvas.height)/2)/100) * 49.5 ;
    }
    
    function initMiddleCircle() {
      middleCircle.x = canvas.width/2;
      middleCircle.y = canvas.height/2;
      middleCircle.radius = (canvas.width + canvas.height) * 0.055;
      middleCircle.color = 'gray';
    middleCircle.touchDistance = 0;
    }
  function renderScene() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    renderMiddleCircle();
    renderFrameCircle();
    sendToWebsocket();
  }
  
    function renderMiddleCircle() {
      ctx.beginPath();
      ctx.arc(middleCircle.x, middleCircle.y, middleCircle.radius, 0, 2 * Math.PI, false);
      ctx.fillStyle = middleCircle.color;
      ctx.fill();
    }
  
  function renderFrameCircle() {
    ctx.beginPath();
    ctx.arc(frameCircle.x, frameCircle.y, frameCircle.radius, 0, 2 * Math.PI, false);
    ctx.lineWidth = 2;
    ctx.stroke();   
  }
    
    // handle events
    canvas.addEventListener("touchstart", handleStart, false);
    canvas.addEventListener("touchend", handleEnd, false);
    canvas.addEventListener("touchmove", handleMove, false);
      
    function handleStart(evt) {
      evt.preventDefault();
      elemLeft = canvas.offsetLeft,
      elemTop = canvas.offsetTop;

      let touches = evt.changedTouches;
      for (var i = 0; i < touches.length; i++) {
        const x = touches[i].pageX - elemLeft, y = touches[i].pageY - elemTop;  
        if ( Math.abs(middleCircle.x - x) < middleCircle.radius && Math.abs(middleCircle.y - y) < middleCircle.radius){
          middleCircle.color = "cornflowerBlue";
          renderScene();
        }
       }
      }
      
      function handleMove(evt) {
        evt.preventDefault();
        elemLeft = canvas.offsetLeft,
        elemTop = canvas.offsetTop;

        let touches = evt.changedTouches;
        for (var i = 0; i < touches.length; i++) {
      const x = touches[i].pageX - elemLeft, 
          y = touches[i].pageY - elemTop,
          centerX = canvas.width/2,
          centerY = canvas.height/2;
          middleCircle.touchDistance = Math.sqrt(Math.pow(x - centerX, 2) + Math.pow(y - centerY, 2));

      // clip middle circle outer frame circle, clip is function performance issue
      if(middleCircle.touchDistance + middleCircle.radius > frameCircle.radius) {
        ctx.clip();
      }
      // middle circle inside frame circle
      if(middleCircle.touchDistance > frameCircle.radius) {
        return;
      }
        //touch inside frame circle
      //if ( Math.abs(middleCircle.x - x) < middleCircle.radius && Math.abs(middleCircle.y - y) < middleCircle.radius){
        middleCircle.x = x;
        middleCircle.y = y;
        renderScene();
      //}
        } 
      }
      
  function handleEnd(evt) {
    evt.preventDefault();
    initMiddleCircle();
    renderScene();
    }
    
    function sendToWebsocket() {      
      const middleHeightCanvas = canvas.height/2,
      middleWidthCanvas = canvas.width/2,
      straight = (-1 * (middleCircle.y - middleHeightCanvas)/( middleHeightCanvas /100)).toFixed(),
      vertical = ((middleCircle.x - middleWidthCanvas)/( middleWidthCanvas /100)).toFixed();
      //console.log(straight, vertical);  
      if(isWSactive) {
        ws.send(straight +" "+ vertical);
      }
    }
  
    ws.onopen = () => {isWSactive = true;};
    ws.onclose = () => { isWSactive = false; console.log("Connection is closed..."); };
   </script>
</html>
)======";

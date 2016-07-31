/* Starting coordinates for both control points */
var cp1 = new Array( 100, 50 );
var cp2 = new Array( 200, 200 );

var moveLimit = 50;     // total number of moves to make
var currentMove = 0;    // current number of moves

var deltaX;             // X movement amount
var deltaY;                         // Y movement amount

var delay = 100;        // one-tenth second

var timer;              // store the alarm clock

function init( evt )
{
    /* do any global initialization here */
}

/*
 * Initialize movement; set control points back to their
 * starting positions, set the current number of moves
 * to zero, and set an interval timer to move the
 * control points every "delay" milliseconds.
 */
function setupMove()
{
    currentMove = 0;
    deltaX = 5;                 /* start X at five pixels per interval */
    deltaY = 0.75;              /* start Y at 3/4 pixels per interval */
    setGraphicInfo();
    timer = setInterval( "moveControlPoints()", delay );
}

function setGraphicInfo()
{  
    var obj;
    var pathString;
    
    /*
     * Construct a path that describes a cubic
     * Bezier curve using the current coordinates
     * for the control points.
     */
     pathString = "M 50 120 C " +
        (cp1[0] + currentMove * deltaX) + " " +
        (cp1[1] - currentMove * deltaY) + ", " +
        (cp2[0] - currentMove * deltaX)  + " " +
        (cp2[1] + currentMove * deltaY) + ", " +
        "250, 120";
    
    obj = svgDocument.getElementById( "cubic" );
    obj.setAttribute( "d", pathString );
}

function moveControlPoints()
{
    currentMove++;      // we've done one more move
    setGraphicInfo();   // adjust the control points and path

    /*
     * If we have finished moving, clear the timer, which
     * will stop the repeated wake-up calls.
     */
    if (currentMove >= moveLimit)
    {
       clearInterval( timer );
    }
}

function stopMovement()
{
    clearInterval( timer );
}

/*
 * setInterval expects its first argument to be found in
 * the main window. We oblige it by creating a reference
 * to this script's moveControlPoints function as a
 * window property
 */
// window.moveControlPoints = moveControlPoints;

var scaleChoice = 1; 
var scaleFactor = new Array( 1.25, 1.5, 1.75 );

function init( evt )
{
    transformShirt();
}

function setScale( n )
{
    obj = svgDocument.getElementById( "scale" + scaleChoice );  
    obj.setAttribute( "fill", "white" );
    scaleChoice = n;
    obj = svgDocument.getElementById( "scale" + scaleChoice );
    obj.setAttribute( "fill", "#ffc" );
    transformShirt();
}

function transformShirt( )
{
    var obj = svgDocument.getElementById( "shirt" );
    obj.setAttribute( "transform",
        "scale(" + scaleFactor[scaleChoice] + ")"
    );
    obj.setAttribute( "stroke-width",
        1 / scaleFactor[scaleChoice] );
}

setScale(0);

function swapLoc1Loc2() {
    var loc1 = document.getElementById("loc1").nodeValue;
    var loc2 = document.getElementById("loc2").nodeValue;

    document.getElementById("loc1").nodeValue = loc2;
    document.getElementById("loc2").nodeValue = loc1;
}
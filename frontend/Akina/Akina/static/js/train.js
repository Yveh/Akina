function swapLoc1Loc2() {
    var loc1 = document.getElementById("loc1").value;
    var loc2 = document.getElementById("loc2").value;

    document.getElementById("loc1").value = loc2;
    document.getElementById("loc2").value = loc1;
}
$(document).ready(function(){
    console.log("init")
    $('#ticketButton').click(function () {
        console.log("ok");
        $('#ticketTable').bootstrapTable('insertRow', {index: 1, row: {type : "<input type=\"text\" class=\"form-control\" name=\"type[]\">"}});
    });
    $('#trainButton').click(function () {
        console.log("ok");
        $('#trainTable').bootstrapTable('insertRow', {index: 1, row: {loc1 : "<input type=\"text\" class=\"form-control\" name=\"loc1[]\">",
                                                                      time1: "<input type=\"time\" name=\"time1[]\">",
                                                                      time2: "<input type=\"time\" name=\"time2[]\">",
                                                                      time3: "<input type=\"time\" name=\"time3[]\">",
                                                                      price: "<input type=\"text\" class=\"form-control\" name=\"price[]\">"}});
    });
})


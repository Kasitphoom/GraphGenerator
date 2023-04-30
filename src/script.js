const Category = document.getElementsByClassName('cat');
var CategoryLength = max_cat_length();
console.log(CategoryLength)

if (CategoryLength > 7){
    for (var i = 0; i < Category.length; i++) {
        var x = Category[i].getAttribute('x');
        var y = Category[i].getAttribute('y');
        var catstyle = Category[i].style;
        console.log(x, y)
        catstyle.transformOrigin = x + 'px ' + y + 'px';
        catstyle.transform = 'rotate(-90deg) translateX(' + Category[i].textContent.length * 1.5 + 'px)';
        catstyle.textAnchor = 'start';
        catstyle.fill = '#282C34';
    }
}

function max_cat_length() {
    var max = 0;
    for (var i = 0; i < Category.length; i++) {
        if (Category[i].innerHTML.trim().length > max) {
            max = Category[i].innerHTML.trim().length;
        }
    }
    return max;
}
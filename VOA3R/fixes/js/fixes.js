(function ($, Drupal, window, document, undefined) {
var $textbox = $("#edit-keys2"),
    searchText = '';

$textbox.keyup(function(event){
    if(event.keyCode == 13){
        event.preventDefault();
        event.stopImmediatePropagation();
        // $("#id_of_button").click();
        // alert(document.location.origin + '/search/site/' + encodeURIComponent($textbox.val()));
        document.location.href = document.location.origin + '/search/site/' + encodeURIComponent($textbox.val());
        
    }
});

})(jQuery, Drupal, this, this.document);
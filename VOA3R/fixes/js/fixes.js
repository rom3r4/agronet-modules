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

// jQuery colorbox responsivenes's fix
if (window.matchMedia) {
    // Establishing media check
    width700Check = window.matchMedia("(max-width: 700px)");
    if (width700Check.matches){
      $.colorbox.remove();
    }
}

})(jQuery, Drupal, this, this.document);
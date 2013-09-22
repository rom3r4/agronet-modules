<?php
// $Id: calendar_block.tpl.php,v 0.1 2011/04/25 12:27:20 skilip Exp $

/**
 * @file
 */
?>
<div id="calendar_block">
  <div id="calendar_row0" class="clear-block">
    <div id="nav">
      <div id="calendar_prev" class="<?php print $prev_class; ?>"><span><?php print $previous_link ;?></span></div>
      <div id="calendar_header"><?php print $header; ?></div>
      <div id="calendar_next" class="<?php print $next_class; ?>"><span><?php print $next_link ;?></span></div>
    </div>
    <?php if (isset($rows_weekdays)): ?>
    <?php foreach ($rows_weekdays as $row) : ?>
      <div<?php print drupal_attributes($row['attributes']); ?>><span><?php print $row['data']; ?></span></div>
    <?php endforeach; ?>
    <?php endif; ?>
  </div>

  <?php foreach ($rows_dates as $row) : ?>
    <div<?php print drupal_attributes($row['attributes']); ?>>

    <?php foreach ($row['data'] as $date) : ?>
      <div<?php print drupal_attributes($date['#attributes']); ?>>
        <?php print drupal_render($date); ?>
      </div>
    <?php endforeach; ?>

    </div>
  <?php endforeach; ?>

  <div class="clear-block"></div>
</div>

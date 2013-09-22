<?php
/**
 * @file
 * test1.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function test1_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function test1_node_info() {
  $items = array(
    'resourceagrisap' => array(
      'name' => t('Resource-AGRISAP'),
      'base' => 'node_content',
      'description' => t('Agrisap Resource'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

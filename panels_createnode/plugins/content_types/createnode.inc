<?php

/**
 * @file
 *
 * Plugin Panels "Create node". 
 */


/**
 * Initial array for CreateNode plugin.
 */
$plugin = array(
  'title' => t('Create node'),
  'content types' => 'panels_createnode_createnode_content_type_content_types',
  'render callback' => 'panels_createnode_content_type_render',  
  'edit form' => 'panels_createnode_content_type_edit_form',
  'admin title' => 'panels_createnode_content_type_admin_title',
  'admin info' => 'panels_createnode_content_type_admin_info',
  'edit text' => t('Edit node'),
);

/**
 * Provide administrative title for CreateNode plugin.
 */
function panels_createnode_content_type_admin_title($subtype, $conf, $context = NULL) {
  $output = t('Create node');
  if ($conf['override_title'] && !empty($conf['override_title_text'])) {
    $output = filter_xss_admin($conf['override_title_text']);
  }
  else {
    if (isset($conf['node_nid'])) {
      $nid = $conf['node_nid'];
      $node = node_load($nid);
      if ($node) {
        $output = $node->type;
      }
      else {
        $output = t('Error');
      }
    }
  }
  return $output;
}

/**
 * Provide administrative info for CreateNode plugin.
 */
function panels_createnode_content_type_admin_info($subtype, $conf, $context = NULL) {
  $block = new stdClass();

  if (isset($conf['node_nid'])) {
    $nid = $conf['node_nid'];
    $node = node_load($nid);
    if ($node) {
      $block->title = $node->title;
      
      $view_mode = $conf['node_form_viewmode'] ? $conf['node_form_viewmode'] : variable_get('panels_createnode_viewmode', 'full');
      $block->content = node_view($node, $view_mode);
    }
    else {
      $block->title = t('Node #!nid could not be found.', array('!nid' => $nid));
      $block->content = '';
    }
  }
  
  return $block;
}

/**
 * Return all content types available for CreateNode plugin.
 */
function panels_createnode_createnode_content_type_content_types() {
  $types = array();

  $nodetypes = node_type_get_types();
  
  foreach (variable_get('panels_createnode_nodetypes', array()) as $nodetype => $enabled) {
    if (!$enabled) continue;
    if (node_access('create', $nodetype)) {
      $info = _panels_createnode_createnode_content_type($nodetypes, $nodetype);
      if ($info) {
        $types[$nodetype] = $info;
      }
    }
  }
  
  return $types;
}

/**
 * Build CreateNode plugin by specified node type.
 */
function _panels_createnode_createnode_content_type($nodetypes, $nodetype) {
  return array(
    'title' => $nodetypes[$nodetype]->name,
    'category' => t('Create node'),
    'icon' => 'createnode.png',
  );
}

/**
 * CreateNode plugin output function.
 */
function panels_createnode_content_type_render($subtype, $conf, $args, $context) {
  if (!isset($conf['node_nid'])) {
    return ;
  }
  $nid = $conf['node_nid'];
  $node = node_load($nid);
  
  $block = new stdClass();
  
  if ($node) {
    $block->title = $node->title;
    $view_mode = $conf['node_form_viewmode'] ? $conf['node_form_viewmode'] : variable_get('panels_createnode_viewmode', 'full');
    $block->content = node_view($node, $view_mode);
    
    if (node_access('update', $node)) {
      $block->admin_links = array(
        array(
          'title' => t('Edit node'),
          'alt' => t("Edit this node"),
          'href' => "node/{$node->nid}/edit",
          'query' => drupal_get_destination(),
        ),
      );
    }
    
  }
  else {
    $block->title = t('Node #!nid could not be found.', array('!nid' => $nid));
    $block->content = '';
  }
  return $block;
}

/**
 * CreateNode plugin form.
 * Attaching node add/edit form.
 */
function panels_createnode_content_type_edit_form($form, &$form_state) {
  global $user, $language;
  
  
  $nodetypes = node_type_get_types();
  $type = $form_state['subtype_name'];
  
  module_load_include('inc', 'node', 'node.pages');
  
  $conf = $form_state['conf'] ? $form_state['conf'] : array();
  
  if (isset($conf['node_nid'])) {
    $nid = $conf['node_nid'];
    $node = node_load($nid);
    
    if (!node_access('update', $node)) {
      return ;
    }
  }
  else {
    $node = new stdClass();
    $node->uid = $user->uid;
    $node->name = isset($user->name) ? $user->name : '';
    $node->type = $type;
    $node->language = $language->language;
    
    if (!node_access('create', $type)) {
      return ;
    }
  }
  
   // Invoke hook_nodapi and hook_node
  node_object_prepare($node);
  
  $form['node_title'] = array('#markup' => '<h3>' . t('Create @name', array('@name' => $nodetypes[$type]->name)) . '</h3>');
  
  $node_form_state['build_info']['args'] = array($node);
  form_load_include($node_form_state, 'inc', 'node', 'node.pages');
  $node_form = drupal_build_form($type . '_node_form', $node_form_state);
  unset($node_form['actions']);
  
  $form['node_form'] = $node_form;
  $form['#token'] = FALSE;

  if (user_access('control node view form')) {
    $form['node_form_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Node view mode settings'),
    );
    
    $view_modes = array();
    $node_entity_info = entity_get_info('node');
    foreach ($node_entity_info['view modes'] as $key => $view_mode) {
      $view_modes[$key] = $view_mode['label'];
    }    
    $form['node_form_settings']['node_form_viewmode'] = array(
      '#type' => 'select',
      '#title' => t('Node view mode'),
      '#default_value' => isset($conf['node_form_viewmode']) ? $conf['node_form_viewmode'] : variable_get('panels_createnode_viewmode', 'full'),
      '#description' => t('View mode, e.g. "full", "teaser"...'),
      '#options' => $view_modes,
    );
  }
  
  return $form;
}

/**
 * CreateNode plugin form submit function.
 */
function panels_createnode_content_type_edit_form_submit($form, &$form_state) {
  $node = (object) $form_state['values'];
  $node->title = $form_state['input']['title'];
  node_save($node);
  
  if ($node->nid) {
    $form_state['conf']['node_nid'] = $node->nid;
    if (user_access('control node view')) {
      $form_state['conf']['node_form_viewmode'] = $form_state['values']['node_form_viewmode'];
    }
  }
}
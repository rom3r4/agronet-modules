<?php

/**
 * @file drush_updater.drush.inc
 * TODO: Enter file description here.
 */


// Todo change this with the proper Data structure
DEFINE('KEY_AGRISAP', 'AGRISAP_FIELDS_STORAGE');
DEFINE('KEY_DC', 'DC_FIELDS_STORAGE');

DEFINE('TITLE_FIELD_AGRISAP', 'field_ag_title');
DEFINE('TITLE_FIELD_DC', 'field_dc_titles');

DEFINE('BODY_FIELD_AGRISAP', 'field_ag_descriptions_value');
DEFINE('BODY_FIELD_DC', 'field_dc_descriptions');

DEFINE('RESOURCE_NAME_AGRISAP', 'resource_agrisap');
DEFINE('RESOURCE_NAME_DC', 'resource_dc');

DEFINE('RESOURCE_ABREB_AGRISAP', 'ag');
DEFINE('RESOURCE_ABREB_DC', 'dc');


$KEY = "";
$TITLE_FIELD = "";
$BODY_FIELD = "";
$RESOURCE = "";
$ABREB = "";

/**
 * Implements hook_drush_command().
 */
function drush_updater_drush_command()
{
  $items = array();
  // callback is the function that will be called when the command is executed
  $items['voa3r-load-data-ag'] = array(
    'callback' => 'drush_drush_updater_helper_ag',
    'description' => 'Loads data into VOA3R-AGRISAP content type',
    'aliases' => array('agup'),
    'examples' => array(
      'drush agup --data=UUENCODED-JSON-DATA-XXXXXXXX' => 'loads data into VOA3R',
    ),
    'options' => array(
      'data' => 'the data in UUECODED JSON format.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  $items['voa3r-create-content-ag'] = array(
    'callback' => 'drush_drush_updater_creator_helper_ag',
    'description' => 'Creates the initial VOA3R-AGRISAP content types staructures(trail-error strategy)',
    'aliases' => array('agc'),
    'examples' => array(
      'drush agc --data=UUENCODED-JSON-DATA-XXXXXXXX' => 'create data content type into VOA3R database',
    ),
    'options' => array(
      'data' => 'the data in UUECODED JSON format.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  $items['voa3r-load-data-dc'] = array(
    'callback' => 'drush_drush_updater_helper_dc',
    'description' => 'Loads data into VOA3R-DUBLIN_CORE content type',
    'aliases' => array('dcup'),
    'examples' => array(
      'drush dcup --data=UUENCODED-JSON-DATA-XXXXXXXX' => 'loads data into VOA3R',
    ),
    'options' => array(
      'data' => 'the data in UUECODED JSON format.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  $items['voa3r-create-content-dc'] = array(
    'callback' => 'drush_drush_updater_creator_helper_dc',
    'description' => 'Creates the initial VOA3R-DUBLIN_CORE content types staructures(trail-error strategy)',
    'aliases' => array('dcc'),
    'examples' => array(
      'drush dcc --data=UUENCODED-JSON-DATA-XXXXXXXX' => 'create data content type into VOA3R database',
    ),
    'options' => array(
      'data' => 'the data in UUECODED JSON format.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  $items['voa3r-reset'] = array(
    'callback' => 'drush_drush_updater_voa3r_reset',
    'description' => 'Creates the initial VOA3R-DUBLIN_CORE content types staructures(trail-error strategy)',
    'aliases' => array('vreset'),
    'examples' => array(
      'drush vreset' => 'recreates data structures on next run (deletes cached arrays)',
    ),
    'options' => array(
      'data' => '',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  return $items;
}

/**
 * Helper function to store current fields in database (as a serialized string)
 * @param array/hash
 */
function store_serialized_helper($array)
{
  global $KEY;
  variable_set($KEY, $array);
}

/**
 * Helper function to store current fields in database (as a serialized string)
 * @param array/hash
 */
function retrieve_unserialized_helper()
{
  global $KEY;
  return variable_get($KEY, array());
}

/**
 * Deletes cached arrays. (recreated content data fields within drupal)
 *
 */
function drush_drush_updater_voa3r_reset()
{
  variable_del(KEY_AGRISAP);
  variable_del(KEY_DC);
  drush_log(dt('done.'), 'success');
}

/**
 * Gets an allowed drupal field name
 *
 * @param $field
 * @return string
 */
function get_key_helper($field)
{
  global $ABREB;

  $field = strtolower($field);
  // drush_print($field);
  // return;
  if (strlen($field) > 0) {
    return $field_field = ($field[0] == '_') ? "field" . "_" . $ABREB . $field : "field_" . $ABREB . "_" . $field;
  } else {
    return "";
  }
}

/**
 * this Helper function created one content-type fiel per each time it is called
 *
 * @param $field
 */
function field_create_helper($field)
{
  global $RESOURCE, $ABREB;

  // drush_print("res: " . $RESOURCE);
  $field = strtolower($field);
  $field_field = get_key_helper($field);

  // drush_print("creando: ".$field_field);

  // Check if our field is not already created.
  if (!field_info_field($field_field)) {
    // drush_print("in!: ".$field_field);
    $new_field = array(
      'active' => 1,
      // 'cardinality' => -1,
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => $field_field,
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => 0,
      'module' => 'text',
      'settings' => array(
        'max_length' => 5120,
      ),
      'translatable' => 0,
      'type' => 'text',
    );
    $res = field_create_field($new_field);
    // drush_print("res: ".$res);


// Create the instance on the bundle.
    $instance = array(
      'field_name' => $field_field,
      'label' => $field,
      'bundle' => $RESOURCE,
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 1,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'required' => 0,
      'settings' => array(
        'text_processing' => 0,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => 60,
        ),
        'type' => 'text_textfield',
        'weight' => 41,
      ),

    );
    $res = field_create_instance($instance);
    // drush_print("res2: ".$res);

  }
}

/**
 * Helper function to detect a serialized JSON is valid.
 * @param JSON
 */
function isJson($string)
{
  json_decode($string);
  return (json_last_error() == JSON_ERROR_NONE);
}

/**
 * DRY Helper function. Proxy pattern
 * @param
 */
function drush_drush_updater_creator_helper_dc()
{
  global $KEY, $TITLE_FIELD, $BODY_FIELD, $RESOURCE, $ABREB;
  $ABREB = RESOURCE_ABREB_DC;
  $KEY = KEY_DC;
  $TITLE_FIELD = TITLE_FIELD_DC;
  $BODY_FIELD = BODY_FIELD_DC;
  $RESOURCE = RESOURCE_NAME_DC;
  drush_drush_updater_creator_helper();
}

/**
 * DRY Helper function. Proxy pattern
 * @param
 */
function drush_drush_updater_creator_helper_ag()
{
  global $KEY, $TITLE_FIELD, $BODY_FIELD, $RESOURCE, $ABREB;
  $ABREB = RESOURCE_ABREB_AGRISAP;
  $KEY = KEY_AGRISAP;
  $TITLE_FIELD = TITLE_FIELD_AGRISAP;
  $BODY_FIELD = BODY_FIELD_AGRISAP;
  $RESOURCE = RESOURCE_NAME_AGRISAP;
  drush_drush_updater_creator_helper();
}


/**
 * Helper drush callback.
 * @param
 */
function drush_drush_updater_creator_helper()
{
  // you need this to autoload the functions to create nodes
  module_load_include('inc', 'node', 'node.pages');


  $data = drush_get_option('data') ?
    explode(',', drush_get_option('data')) : array();

  $i = 0;
  // do some business logic to load your data from where it is
  foreach ($data as $res) {

    // drush_print($res);
    $decoded = base64_decode($res);
    if ($decoded) {
      if ($i == 0) {


        $array = (array)json_decode($decoded);

        // reset key storage
        // variable_del(KEY_AGRISAP);

        // $arr = array();
        $arr = retrieve_unserialized_helper();
        foreach ($array as $key => $val) {

          if (array_key_exists($key, $arr)) {
            // drush_print("hit!");
          } else {
            field_create_helper($key);
            // drush_print("miss");
          }

          $arr[$key] = $key;
        }
        store_serialized_helper($arr);
        // drush_print(print_r($arr, TRUE));
        // drush_print("---> ".count($arr));
        // TODO: PHPUnit: assert count($arr) is bigger or equal on each iteration
      }
    }
    $i++;
  }

  drush_log(dt('done.'), 'success');
}

/**
 * DRY Helper function. Proxy pattern
 * @param
 */
function drush_drush_updater_helper_dc()
{
  global $KEY, $TITLE_FIELD, $BODY_FIELD, $RESOURCE, $ABREB;
  $ABREB = RESOURCE_ABREB_DC;
  $KEY = KEY_DC;
  $TITLE_FIELD = TITLE_FIELD_DC;
  $BODY_FIELD = BODY_FIELD_DC;
  $RESOURCE = RESOURCE_NAME_DC;

  // drush_print(' key:'.$KEY.' tile'. $TITLE_FIELD.' body:'. $BODY_FIELD .' resume:'.$RESOURCE);

  drush_drush_updater_helper();
}

/**
 * DRY Helper function. Proxy pattern
 * @param
 */
function drush_drush_updater_helper_ag()
{
  global $KEY, $TITLE_FIELD, $BODY_FIELD, $RESOURCE, $ABREB;
  $ABREB = RESOURCE_ABREB_AGRISAP;
  $KEY = KEY_AGRISAP;
  $TITLE_FIELD = TITLE_FIELD_AGRISAP;
  $BODY_FIELD = BODY_FIELD_AGRISAP;
  $RESOURCE = RESOURCE_NAME_AGRISAP;
  drush_drush_updater_helper();
}


/**
 * Helper drush callback.
 * @param
 */
function drush_drush_updater_helper()
{
  global $KEY, $TITLE_FIELD, $BODY_FIELD, $RESOURCE, $user;
  // drush_drush_updater_creator_helper();
  // you need this to autoload the functions to create nodes
  module_load_include('inc', 'node', 'node.pages');

  $data = drush_get_option('data') ?
    explode(',', drush_get_option('data')) : array();


  // $user_ids = array(1, 2, 3, 4, 5, 6, 7);
  // $random_id = array_rand($user_ids);
  // $random_id = 6;
  
  // Content creator D fixed to admin user, Avoidin so problems with databes migration
  $random_id = 1;

  $i = 0;
  // do some business logic to load your data from where it is
  foreach ($data as $res) {


    // drush_print($res);
    $decoded = base64_decode($res);
    if ($decoded) {
      if ($i == 0) {


        $array = (array)json_decode($decoded);

        // drush_print(print_r($array, TRUE));

        // reset key storage
        // variable_del(KEY_AGRISAP);


        $node = new stdClass();
        $node->type = $RESOURCE;
        $node->title = "Title";
        node_object_prepare($node);
        $node->promote = 1;

        $node->language = LANGUAGE_NONE;

        // admin
        $node->uid = $random_id;
        // $node->uid = $user->uid;

        $node->status = 1; //(1 or 0): published or not
        $node->promote = 0; //(1 or 0): promoted to front page
        $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write

        $node->body[$node->language][0]['value'] = 'text';
        $node->body[$node->language][0]['summary'] = 'summary';
        // $node->body['und'][0]['format'] = 'filtered_html';


        // drush_print(print_r($node, TRUE));

        $arr = retrieve_unserialized_helper();
        foreach ($array as $key => $val) {

          if (array_key_exists($key, $arr)) {
            // drush_print("hit!");
          } else {
            field_create_helper($key);
            // drush_print("miss");
          }
          // drush_print(" key: ".$key." val: ".$val);


          $j = 0;
          $key_name = get_key_helper($key);
          foreach ((array)$val as $k => $v) {

            // Try to set your custom field
            $node->{$key_name}[$node->language][$j]['value'] = $v;

            // since there is only 1 title --> O(1)
            if ($TITLE_FIELD == $key_name) {
              $node->title = $v;
              $node->body[$node->language][0]['summary'] = $v;
            } else if ($BODY_FIELD == $key_name) {
              $node->body[$node->language][0]['value'] = $v;
            }


            $j++;
          }
          $arr[$key] = $key;
        }
        store_serialized_helper($arr);
        // drush_print(print_r($arr, TRUE));
        // TODO: PHPUnit: assert count($arr) is bigger or equal on each iteration


        if ($node = node_submit($node)) { // Prepare node for saving
          node_save($node);
          // drush_print("saved!");
        }

      }
    }
    $i++;
  }

  drush_log(dt('done.'), 'success');
}

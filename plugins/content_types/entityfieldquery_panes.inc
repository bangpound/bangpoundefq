<?php

/**
 * @file
 * Content type plugin to allow Views to be exposed as a display type,
 * leaving most of the configuration on the view.
 */

$plugin = array(
  'title' => t('EntityFieldQuery'),
  'content type' => 'bangpoundefq_entityfieldquery_panes_content_type_content_type',
);

function bangpoundefq_entityfieldquery_panes_content_type_content_type($subtype_id) {
  $plugin = ctools_get_plugins('bangpoundefq', 'queries', $subtype_id);
  return _bangpoundefq_entityfieldquery_panes_content_type_content_type($plugin);
}

/**
 * Return all custom content types available.
 */
function bangpoundefq_entityfieldquery_panes_content_type_content_types() {
  $types = array();
  foreach (ctools_get_plugins('bangpoundefq', 'queries') as $id => $plugin) {
    $types[$id] = _bangpoundefq_entityfieldquery_panes_content_type_content_type($plugin);
  }

  return $types;
}

/**
 * Output function for the 'views' content type.
 *
 * Outputs a view based on the module and delta supplied in the configuration.
 */
function bangpoundefq_entityfieldquery_panes_content_type_render($subtype, $conf, $panel_args, $contexts) {
  if (!is_array($contexts)) {
    $contexts = array($contexts);
  }

  $class = ctools_plugin_load_class('bangpoundefq', 'queries', $subtype, 'handler');
  $query = new $class($conf);

  $output = array();
  foreach ($query->execute() as $entity_type => $stubs) {
    $entities = entity_load($entity_type, array_keys($stubs));
    $output['content'][] = entity_view($entity_type, $entities, $conf['view_mode'][$entity_type]);
  }
  $output['pager'] = array(
    '#markup' => theme('pager'),
    '#weight' => 10,
  );

  $block = new stdClass();
  $block->content = $output;
  $block->module = 'entityfieldquery';
  $block->delta  = 0;
  $block->title = $conf['title'];

  if (!empty($conf['more'])) {
    $block->more = array(
      'href' => $conf['more'],
    );
  }

  return $block;
}

/**
 * Return an info array for a specific custom content type.
 */
function _bangpoundefq_entityfieldquery_panes_content_type_content_type($plugin) {
  return array(
    'name' => $plugin['name'],
    'title' => $plugin['title'],
    'category' => t('EntityFieldQuery'),
    'handler' => $plugin['name'],
    'no title override' => TRUE,
    'defaults' => array(
      'admin_title' => '',
      'title' => '',
      'substitute' => TRUE,
      'view_mode' => 'full',
      'more' => '',
    ) + $plugin['defaults'],
  );
}

/**
 * Returns an edit form for a block.
 */
function bangpoundefq_entityfieldquery_panes_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $query_plugin = ctools_get_plugins('bangpoundefq', 'queries', $form_state['subtype']['name']);

  $form['admin_title'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($conf['admin_title']) ? $conf['admin_title'] : '',
    '#title' => t('Administrative title'),
    '#description' => t('This title will be used administratively to identify this pane. If blank, the regular title will be used.'),
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['title'],
    '#title' => t('Title'),
  );

  if (!empty($form_state['contexts'])) {
    // Set extended description if both CCK and Token modules are enabled, notifying of unlisted keywords
    if (module_exists('token')) {
      $description = t('If checked, context keywords will be substituted in this content. Note that CCK fields may be used as keywords using patterns like <em>%node:field_name-formatted</em>.');
    }
    elseif (!module_exists('token')) {
      $description = t('If checked, context keywords will be substituted in this content. More keywords will be available if you install the Token module, see http://drupal.org/project/token.');
    }
    else {
      $description = t('If checked, context keywords will be substituted in this content.');
    }

    $form['substitute'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use context keywords'),
      '#description' => $description,
      '#default_value' => !empty($conf['substitute']),
    );
    $form['contexts'] = array(
      '#title' => t('Substitutions'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $rows = array();
    foreach ($form_state['contexts'] as $context) {
      foreach (ctools_context_get_converters('%' . check_plain($context->keyword) . ':', $context) as $keyword => $title) {
        $rows[] = array(
          check_plain($keyword),
          t('@identifier: @title', array('@title' => $title, '@identifier' => $context->identifier)),
        );
      }
    }
    $header = array(t('Keyword'), t('Value'));
    $form['contexts']['context'] = array('#markup' => theme('table', array('header' => $header, 'rows' => $rows)));
  }

  $form['more'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['more'],
    '#title' => t('More link'),
  );

  $class = ctools_plugin_load_class('bangpoundefq', 'queries', $form_state['subtype_name'], 'handler');

  $form['query'] = array();
  $form['query'] = $class::editForm($form['query'], $form_state);
  $form['query']['#tree'] = FALSE;

  $form['view_mode'] = array(
    '#type' => 'fieldset',
    '#title' => t('View modes'),
    '#tree' => TRUE,
    '#description' => 'Select the view mode for the entity types that may be returned.',
  );
  $entity_info = entity_get_info();
  foreach ($entity_info as $entity_type => $info) {

    // Temporarily only support one entity type.
    if ($entity_type != $class::entityType()) {
      continue;
    }

    $options = array();
    if (!empty($info['view modes'])) {
      foreach ($info['view modes'] as $mode => $settings) {
        $options[$mode] = $settings['label'];
      }
    }

    if (count($options) > 1) {
      $form['view_mode'][$entity_type] = array(
        '#type' => 'select',
        '#options' => $options,
        '#title' => $info['label'],
        '#default_value' => isset($conf['view_mode'][$entity_type]) ? $conf['view_mode'][$entity_type] : 'default',
      );
    }
    else {
      $form['view_mode'][$entity_type] = array(
        '#type' => 'value',
        '#value' => $options ? key($options) : 'default',
      );
    }
  }

  return $form;
}


/**
 * Store form values in $conf.
 */
function bangpoundefq_entityfieldquery_panes_content_type_edit_form_validate(&$form, &$form_state) {
  $class = ctools_plugin_load_class('bangpoundefq', 'queries', $form_state['subtype_name'], 'handler');

  $class::validateForm($form['query'], $form_state);
}

/**
 * Store form values in $conf.
 */
function bangpoundefq_entityfieldquery_panes_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['subtype']['defaults']) as $key) {
    $value = isset($form_state['values'][$key]) ? $form_state['values'][$key] : $form_state['subtype']['defaults'][$key];
    if ($key == 'bundles') {
      foreach ($value as $entity_type => &$bundles) {
        $bundles = array_keys(array_filter($bundles));
      }
    }
    $form_state['conf'][$key] = $value;
  }
}


/**
 * Returns the administrative title for a type.
 */
function bangpoundefq_entityfieldquery_panes_content_type_admin_title($subtype, $conf, $contexts) {

  $output = t('EntityFieldQuery');
  $title = !empty($conf['admin_title']) ? $conf['admin_title'] : $conf['title'];
  if ($title) {
    $output = t('EntityFieldQuery: @title', array('@title' => $title));
  }

  return $output;
}

/**
 * Returns the administrative title for a type.
 */
function bangpoundefq_entityfieldquery_panes_content_type_admin_info($subtype, $conf, $contexts) {
  $info = array();

  $block = new stdClass;
  $block->title = $conf['title'];
  $block->content = '<pre>'. json_encode($conf) .'</pre>';
  return $block;
}

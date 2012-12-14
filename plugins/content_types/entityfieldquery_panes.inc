<?php

/**
 * @file
 * Content type plugin to allow Views to be exposed as a display type,
 * leaving most of the configuration on the view.
 */

$plugin = array(
  'title' => t('EntityFieldQuery'),
  'description' => t('A simple query.'),
  'category' => t('Content'),
  'no title override' => TRUE,
  'defaults' => array(
    'admin_title' => '',
    'title' => '',
    'substitute' => TRUE,
    'view_mode' => 'full',
  ),
);

function bangpoundefq_entityfieldquery_panes_content_type_content_types($plugin) {
  $types = array();
  foreach (ctools_get_plugins('bangpoundefq', 'queries') as $query) {
    $types[$query['name']] = BangpoundEFQContent::info($query);
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

  $handler = new BangpoundEFQContent($subtype, $conf);

  $block = new stdClass();
  $block->content = $handler->render($conf['view_mode']);
  $block->module = 'entityfieldquery';
  $block->delta  = 0;
  $block->title = $conf['title'];

  return $block;
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

  $form = BangpoundEFQContent::editForm($form, $form_state);

  return $form;
}

/**
 * Store form values in $conf.
 */
function bangpoundefq_entityfieldquery_panes_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array('plugin', 'subtype') as $plugin) {
    foreach (array_keys($form_state[$plugin]['defaults']) as $key) {
      $form_state['conf'][$key] = isset($form_state['values'][$key]) ? $form_state['values'][$key] : $form_state[$plugin]['defaults'][$key];
    }
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
  $block->title = 'Title';
  $block->content = 'Content';
  return $block;
}

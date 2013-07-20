<?php

/**
 * @file
 *
 * Plugin to provide a node context. A node context is a node wrapped in a
 * context object that can be utilized by anything that accepts contexts.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("EntityFieldQuery"),
  'description' => t('EntityFieldQuery object.'),
  'context' => 'bangpoundefq_context_entityfieldquery_create',

  'edit form' => 'bangpoundefq_context_entityfieldquery_settings_form',
  'edit form validate' => 'bangpoundefq_context_view_settings_form_validate',
  'edit form submit' => 'bangpoundefq_context_view_settings_form_submit',

  'defaults' => array('query' => ''),

  'keyword' => 'query',
  'context name' => 'query',

  'get child' => 'bangpoundefq_context_entityfieldquery_get_child',
  'get children' => 'bangpoundefq_context_entityfieldquery_get_children',
);

function bangpoundefq_context_entityfieldquery_get_child($plugin, $parent, $child) {
  $plugins = bangpoundefq_context_entityfieldquery_get_children($plugin, $parent);
  return $plugins[$parent . ':' . $child];
}

function bangpoundefq_context_entityfieldquery_get_children($plugin, $parent) {
  $plugins = array();
  foreach (ctools_get_plugins('bangpoundefq', 'queries') as $query_plugin) {
    $class = ctools_plugin_get_class($query_plugin, 'handler');
    $plugins['entityfieldquery:'. $query_plugin['name']] = _bangpoundefq_get_context_from_query($query_plugin, $parent, FALSE);
  }
  return $plugins;
}

/**
 * Get the child plugin for a view context display.
 *
 * This can return both the context and relationship style. The
 * $required parameter is used to distinguish if context is required
 * or not, so we know whether we need it suitable as a pure context
 * (i.e, no context required) or a relationship (i.e, context required).
 */
function _bangpoundefq_get_context_from_query($plugin, $parent, $required = TRUE) {
  $title = $plugin['title'];

  $description = $plugin['description'];
  $contexts = array();

  $arguments = array();
  foreach ($arguments as $argument) {
    $argument['label'] = $argument['name'] ? $argument['name'] : '';
    $contexts[] = '';//ctools_views_get_argument_context($argument);
  }

  $pass = FALSE;
  if ($required) {
    // If context is required, make sure we have at least one optional
    // or required context.
    foreach ($contexts as $context) {
      if (is_object($context)) {
        $pass = TRUE;
        break;
      }
    }

    if (!$pass) {
      return;
    }
  }
  else {
    // If context is not required, then having any required context
    // causes a fail.
    foreach ($contexts as $context) {
      if (is_object($context) && get_class($context) == 'ctools_context_required') {
        return;
      }
    }

    // Since we know we don't want contexts, we an unset this now.
    $contexts = array();
  }

  if ($required) {
    $function = 'views_content_view_from_argument_context';
  }
  else {
    $function = 'bangpoundefq_context_entityfieldquery_create';
  }

  return array(
    'title' => 'EntityFieldQuery: '. $title,
    'description' => filter_xss_admin($description),
    'required context' => $contexts,
    'keyword' => 'query',
    'context' => $function,
    'context name' => $plugin['name'],
    'name' => $parent . ':' . $plugin['name'],
  );
}

function bangpoundefq_context_entityfieldquery_settings_form($form, &$form_state) {
  $conf = &$form_state['conf'];
  $plugin = &$form_state['plugin'];
  list($parent, $child) = explode(':', $plugin['name']);

  $query_plugin = ctools_get_plugins('bangpoundefq', 'queries', $child);
  $form = $query_plugin['handler']::editForm($form, $form_state);

  return $form;
}

function bangpoundefq_context_entityfieldquery_settings_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = isset($form_state['values'][$key]) ? $form_state['values'][$key] : $form_state['plugin']['defaults'][$key];
  }
}

function bangpoundefq_context_entityfieldquery_create($empty, $data = NULL, $conf = FALSE, $plugin = array()) {
  $context = new ctools_context('entityfieldquery');
  $context->plugin = 'entityfieldquery';

  if ($empty) {
    return $context;
  }

  if ($conf) {
    list($parent, $child) = explode(':', $plugin['name']);
    $class = ctools_plugin_get_class('bangpoundefq', 'queries', $child, 'handler');

    $query = new $class($data);
    $context->data = $query->execute();
  }

  if (is_object($data)) {
    return $context;
  }
}

/**
 * Provide a list of ways that this context can be converted to a string.
 */
function bangpoundefq_context_entityfieldquery_convert_list() {
  $list = array(
  );

  return $list;
}

/**
 * Convert a context into a string.
 */
function bangpoundefq_context_entityfieldquery_convert($context, $type) {
  switch ($type) {
  }
}


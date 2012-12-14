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
  'get child' => 'bangpoundefq_context_entityfieldquery_get_child',
  'get children' => 'bangpoundefq_context_entityfieldquery_get_children',
);

function bangpoundefq_context_entityfieldquery_get_child($plugin, $parent, $child) {
  $plugins = bangpoundefq_context_entityfieldquery_get_children($plugin, $parent);
  return $plugins[$parent . ':' . $child];
}

function bangpoundefq_context_entityfieldquery_get_children($plugin, $parent) {
  $plugins = array();
  foreach (ctools_get_plugins('bangpoundefq', 'queries') as $query) {
    $plugins['entityfieldquery:'. $query['name']] = $query['handler']::info($plugin, $parent);
  }
  return $plugins;
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
    $query_plugin = ctools_get_plugins('bangpoundefq', 'queries', $child);

    $query = new $query_plugin['handler']($data);
    $context->data = $query->execute();
  }

  if (is_object($data)) {
    return $context;
  }
}

/**
 * Provide a list of ways that this context can be converted to a string.
 */
function views_content_context_entityfieldquery_convert_list() {
  $list = array(
  );

  return $list;
}

/**
 * Convert a context into a string.
 */
function views_content_context_entityfieldquery_convert($context, $type) {
  switch ($type) {
  }
}


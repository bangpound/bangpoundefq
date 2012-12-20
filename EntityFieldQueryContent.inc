<?php

interface EntityFieldQueryContent {
  static function info(array $plugin);
  static function editForm($form, &$form_state);
}

class BangpoundEFQContent implements EntityFieldQueryContent {
  protected $query;
  protected $conf;
  protected $subtype;

  function __construct($subtype, $conf) {
    $this->subtype = $subtype;
    $this->conf = $conf;
    $query_plugin = ctools_get_plugins('bangpoundefq', 'queries', $subtype);
    $this->query = new $query_plugin['handler']($conf);
  }

  static function info(array $plugin) {
    return array(
      'category' => t('EntityFieldQuery'),
      'handler' => $plugin['name'],
    ) + $plugin;
  }

  static function editForm($form, &$form_state) {
    $conf = $form_state['conf'];

    $query_plugin = ctools_get_plugins('bangpoundefq', 'queries', $form_state['subtype_name']);
    $entity_type = $query_plugin['handler']::entityType();
    $entity_info = entity_get_info($entity_type);

    $options = array();
    if (!empty($entity_info['view modes'])) {
      foreach ($entity_info['view modes'] as $mode => $settings) {
        $options[$mode] = $settings['label'];
      }
    }

    $form['query'] = $query_plugin['handler']::editForm($form['query'], $form_state);
    $form['query']['#tree'] = FALSE;

    if (count($options) > 1) {
      $form['view_mode'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#title' => t('View mode'),
        '#default_value' => $conf['view_mode'],
      );
    }
    else {
      $form['view_mode_info'] = array(
        '#type' => 'item',
        '#title' => t('View mode'),
        '#description' => t('Only one view mode is available for this entity type.'),
        '#markup' => $options ? current($options) : t('Default'),
      );

      $form['view_mode'] = array(
        '#type' => 'value',
        '#value' => $options ? key($options) : 'default',
      );
    }

    return $form;
  }

  static function validateForm(&$form, &$form_state) {
    $query_plugin = ctools_get_plugins('bangpoundefq', 'queries', $form_state['subtype_name']);
    $query_plugin['handler']::validateForm($form['query'], $form_state);
  }

  function render($view_mode = 'full') {
    $output = array();
    foreach ($this->query->execute() as $entity_type => $stubs) {
      $entities = entity_load($entity_type, array_keys($stubs));
      $output['content'][] = entity_view($entity_type, $entities, $view_mode);
    }
    $output['pager'] = array(
      '#markup' => theme('pager'),
      '#weight' => 10,
    );
    return $output;
  }
}

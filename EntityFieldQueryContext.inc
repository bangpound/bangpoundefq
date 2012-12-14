<?php

interface EntityFieldQueryContext {
  static function info(array $plugin, $parent);
  static function editForm($form, &$form_state);
}

interface EntityFieldQueryContent {
  static function info(array $plugin);
  static function editForm($form, &$form_state);
}
/*
class BangpoundEFQContext implements EntityFieldQueryContext {
  protected $query;

  static function info(array $plugin, $parent) {
    $info['title'] = 'Node list';
    $info['keyword'] = 'node_list';
    $info['context name'] = 'node_list';
    $info['name'] = $parent . ':node_list';
    $info['description'] = t('Creates EFQ context from @class.', array('@class' => 'node_list'));
  }

  static function editForm($form, &$form_state) {
    $conf = $form_state['conf'];
    return $form;
  }
}
*/
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

    $form['query'] = $query_plugin['handler']::editForm($form, $form_state) + array(
      '#tree' => FALSE,
    );

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

  function render($view_mode = 'full') {
    $output = array();
    foreach ($this->query->execute() as $entity_type => $entities) {
      $output[] = entity_view($entity_type, $entities, $view_mode);
    }
    return $output;
  }
}

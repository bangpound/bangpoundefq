<?php

/**
 * Class EntityListHandlerMenuTree
 *
 * @see  menu_tree_collect_node_links
 */
class EntityListHandlerMenuTree implements EntityListHandler {

  protected $config = array();

  protected $arguments = array();

  protected $parameters = array();

  protected $range = array('offset' => NULL, 'limit' => NULL);

  /**
   * Constructor to setup and configure the handler.
   */
  public function __construct(Array $config = array()) {
    $defaults = array(
      'entity_type' => 'node',
    );
    $this->config = array_merge($defaults, $config);
  }

  /**
   * Returns what entity type this list lists.
   *
   * @return string
   */
  public function entityType() {
    return $this->config['entity_type'];
  }

  /**
   * Returns the required arguments needed for this list. Required arguments are
   * defined as CTools context keys.
   *
   * It should be in the following form:
   * @code
   *   return array(
   *     'node.nid' => array(
   *       'label' => t('Node NID'),
   *       'required' => TRUE,
   *     ),
   *   );
   * @endcode
   *
   * The key 'node.nid' is a ctools context key. It can be in any of the
   * following formats:
   *   - node
   *   - node.nid
   *   - entity:taxonomy_term
   *   - entity:taxonomy_term.tid
   *
   * @return array
   */
  public function requiredArguments() {
    return array();
  }

  /**
   * Setter for the list's arguments.
   *
   * @return $this
   */
  public function setArguments(Array $arguments = array()) {
    // TODO: Implement setArguments() method.
  }

  /**
   * Getter for the list's arguments. Mostly used internally.
   *
   * @return array
   */
  public function getArguments() {
    return $this->arguments;
  }

  /**
   * Setter for the list's arguments.
   *
   * @return $this
   */
  public function setParameters(Array $parameters = array()) {
    // TODO: Implement setParameters() method.
  }

  /**
   * Getter for the list's parameters.
   *
   * @return array
   */
  public function getParameters() {
    return $this->parameters;
  }

  /**
   * Setter for the list's range.
   *
   * @return $this
   */
  public function setRange($offset = NULL, $limit = NULL) {
    // TODO: Implement setRange() method.
  }

  /**
   * Getter for the list's range.
   *
   * @return array
   */
  public function getRange() {
    return $this->range;
  }

  /**
   * Build the handler's query.
   */
  public function buildQuery() {
    // TODO: Implement buildQuery() method.
  }

  /**
   * Returns the number of entities in this list.
   *
   * @return int
   */
  public function count() {
    // TODO: Implement count() method.
  }

  /**
   * Getter for the cache tags that applies to this list.
   *
   * @return array
   */
  public function cacheTags() {
    // TODO: Implement cacheTags() method.
  }

  /**
   * Execute the handler query. This function should return a numeric array of
   * entities, simply keyed by the result index.
   *
   * @see EntityFieldQuery::execute()
   * @return array
   */
  public function execute() {
    $config = array(
      'menu_name'   => 'navigation',
      'parent_mlid' => 0,
      'parent'      => '',
      'title_link'  => 0,
      'admin_title' => '',
      'level'       => 1,
      'follow'      => 1,
      'depth'       => 0,
      'expanded'    => 0,
      'sort'        => 0,
    );
    $tree = menu_tree_block_data($config);
    $entity_links = array();
    bangpoundefq_menu_tree_collect_entity_links($tree, $entity_links, $this->entityType());
    $entities = entity_load($this->entityType(), array_keys($entity_links));
    return $entities;
  }

  /**
   * Returns the path to the administrative interface for this list.
   *
   * @return string
   */
  public function adminPath() {
    // TODO: Implement adminPath() method.
  }

  /**
   * Form to configure the handler.
   */
  public function configForm(&$form_state) {
    foreach (entity_get_info() as $entity_type => $info) {
      $options[$entity_type] = $info['label'];
    }
    $form = array();
    $form['entity_type'] = array(
      '#type' => 'radios',
      '#title' => t('Entity type'),
      '#default_value' => $this->config['entity_type'],
      '#options' => $options,
      '#required' => TRUE,
    );
    return $form;
  }
}
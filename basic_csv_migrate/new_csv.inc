<?php
class NewCSVMigration extends Migration {

  public function __construct($arguments) {
    parent::__construct($arguments);

    // For CSV migrations let PHP handle platform-specific line-endings
    ini_set('auto_detect_line_endings', TRUE);

    // The Description of the import. This desription is shown on the Migrate GUI
    $this->description = t('Import of items from a CSV to Article content type.');

    // The definition of the columns. Keys are integer column positions. values are array(field name, description).
    $columns = array(
      0 => array('ID', 'ID'), // source field (does not have to be field 0)
      5 => array('Title', t('Title')),
      7 => array('Content', t('Content')),
    );

    // The source (CSV file) of the import
    $csvfile = drupal_get_path('module', 'basic_csv_migrate') . '/new_items.csv';
    $this->source = new MigrateSourceCSV($csvfile, $columns, array('delimiter' => '|', 'header_rows' => 1));

    // Source and destination relation for rollbacks
    // Set up the key (source) field for rollbacks
    $source_key = array(
      'ID' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'alias' => 'import'
      )
    );

    // Assign the source key to the Migration Map
    $this->map = new MigrateSQLMap(
      $this->machineName,
      $source_key,
      MigrateDestinationNode::getKeySchema()
    );

    // The destination node-type bundle
    $language = 'en';
    $text_format = 'full_html';
    $node_options = MigrateDestinationNode::options($language, $text_format);
    $this->destination = new MigrateDestinationNode('article', $node_options);

    // Field mappings

    // Map to Drupal fields
    $this->addFieldMapping('title', 'Title');
    $this->addFieldMapping('title_field', 'Title');

    // Map to Content Type fields
    $this->addFieldMapping('body', 'Content');

    // Assume everything is published
    $this->addFieldMapping('status')->defaultValue(1);
  }

  /**
   * Prepare and modify the source rows.
   *
   * @param array $row
   *   Row to import for the migration.
   */
  public function prepareRow($row) {
  }

  /**
   * Prepare some of the rows which require extra logic before can be mapped.
   */
  public function prepare($entity, stdClass $row) {
    return $entity;
  }
}

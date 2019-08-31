DROP TABLE IF EXISTS raft_log;
CREATE TABLE raft_log(
  id SERIAL,
  log_time TIMESTAMP NOT NULL,
  severity VARCHAR NOT NULL,
  message VARCHAR NOT NULL,
  node_id VARCHAR NOT NULL
);

CREATE UNIQUE INDEX idx_log_time on raft_log(log_time, node_id);
CREATE INDEX idx_message on raft_log(message);

select log_time, msg_a, msg_b, msg_c, msg_d, msg_e from (
	select log_time, message as msg_a, null as msg_b, null as msg_c,
	    null as msg_d, null as msg_e
        from raft_log
	where node_id = 'pi-3-a'
	union all
	select log_time, null, message, null, null, null from raft_log
	where node_id = 'pi-3-b'
	union all
	select log_time, null, null, message, null, null from raft_log
	where node_id = 'pi-3-c'
	union all
	select log_time, null, null, null, message, null from raft_log
	where node_id = 'pi-3-d'
	union all
	select log_time, null, null, null, null, message from raft_log
	where node_id = 'pi-3-e'
) as all_of_it
where log_time between '2019-08-25 14:26:00' and '2019-08-25 14:26:01';

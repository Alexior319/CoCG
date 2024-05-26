(define (problem Box-3)
	(:domain Box-3)
	(:goal
		(and (box_at b0 p1_2) (box_at b1 p2_2) (box_at b2 p3_2))
	)
	(:init
		(agent_at a1 p1_1)
		(agent_at a2 p3_1)
		(agent_at a3 p3_1)

		(adj p1_1 p1_2)
		(adj p2_1 p2_2)
		(adj p3_1 p3_2)
		(adj p1_2 p1_1)
		(adj p2_2 p2_1)
		(adj p3_2 p3_1)

		(adj p1_1 p2_1)
		(adj p2_1 p3_1)

		(adj p2_1 p1_1)
		(adj p3_1 p2_1)

		(light b0)
		(heavy b1)
		(light b2)
		(unknown
			(box_at b0 p1_1))
		(unknown
			(box_at b0 p1_2))
		(oneof
			(box_at b0 p1_1)
			(box_at b0 p1_2))
		(unknown
			(box_at b1 p2_1))
		(unknown
			(box_at b1 p2_2))
		(oneof
			(box_at b1 p2_1)
			(box_at b1 p2_2))
		(unknown 
			(box_at b2 p3_1))
		(unknown
			(box_at b2 p3_2))
		(oneof
			(box_at b2 p3_1)
			(box_at b2 p3_2))

		(diff_agent a1 a2)
		(diff_agent a2 a1)
		(diff_agent a1 a3)
		(diff_agent a3 a1)
		(diff_agent a2 a3)
		(diff_agent a3 a2)
	)
)
(define (problem Box-4)
	(:domain Box-4)

	(:goal
		(and (box_at b0 p1_2) (box_at b1 p3_2) (box_at b2 p5_2))
	)
	(:init
		(agent_at a1 p1_1)
		(agent_at a2 p5_1)

		(adj p1_1 p1_2)
		(adj p2_1 p2_2)
		(adj p3_1 p3_2)
		(adj p4_1 p4_2)
		(adj p5_1 p5_2)

		(adj p1_2 p1_1)
		(adj p2_2 p2_1)
		(adj p3_2 p3_1)
		(adj p4_2 p4_1)
		(adj p5_2 p5_1)

		(adj p1_1 p2_1)
		(adj p2_1 p3_1)
		(adj p3_1 p4_1)
		(adj p4_1 p5_1)

		(adj p2_1 p1_1)
		(adj p3_1 p2_1)
		(adj p4_1 p3_1)
		(adj p5_1 p4_1)

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
			(box_at b1 p3_1))
		(unknown
			(box_at b1 p3_2))
		(oneof
			(box_at b1 p3_1)
			(box_at b1 p3_2))
		(unknown
			(box_at b2 p5_1))
		(unknown
			(box_at b2 p5_2))
		(oneof
			(box_at b2 p5_1)
			(box_at b2 p5_2))

		(diff_agent a1 a2)
		(diff_agent a2 a1)
	)
)
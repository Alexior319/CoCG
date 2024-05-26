(define (problem Box-7)
	(:domain Box-7)
	(:goal
		(and (box_at b0 p1_2) (box_at b1 p2_2))
	)
	(:init
		; (and
		(agent_at a1 p1_1)
		(agent_at a2 p1_1)

		(adj p1_1 p1_2)
		(adj p2_1 p2_2)

		(adj p1_2 p1_1)
		(adj p2_2 p2_1)

		(adj p1_1 p2_1)
		(adj p2_1 p1_1)

		(heavy b0)
		(heavy b1)
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

		(diff_agent a1 a2)
		(diff_agent a2 a1)
		; )
	)
)
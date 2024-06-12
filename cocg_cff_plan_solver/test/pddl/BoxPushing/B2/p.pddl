(define (problem BP-R2-H1)
	(:domain Box-2)

	(:goal
		(and (box_at b0 p1_2))
	)
	(:init
		(agent_at a1 p1_1)
		(agent_at a2 p1_1)

		(adj p1_1 p1_2)
		(adj p1_2 p1_1)

		(heavy b0)
		(unknown
			(box_at b0 p1_1))
		(unknown
			(box_at b0 p1_2))
		(oneof
			(box_at b0 p1_1)
			(box_at b0 p1_2))

		(diff_agent a1 a2)
		(diff_agent a2 a1)
	)
)
(define (problem Button-3)
	(:domain Button-3)
	(:goal
		(and (Button_pushed b0) (Button_pushed b1))
	)
	(:init
		(agent_at a1 p1_1)
		(agent_at a2 p2_1)

		(adj p1_1 p1_2)
		(adj p2_1 p2_2)
		(adj p1_2 p1_1)
		(adj p2_2 p2_1)

		(adj p1_1 p2_1)
		(adj p2_1 p1_1)

		(green b0)
		(yellow b1)

		(unknown
			(Button_at b0 p1_1))
		(unknown
			(Button_pushed b0))
		(oneof
			(Button_at b0 p1_1)
			(Button_pushed b0))
		(unknown
			(Button_at b1 p2_1))
		(unknown
			(Button_pushed b1))
		(oneof
			(Button_at b1 p2_1)
			(Button_pushed b1))

		(diff_agent a1 a2)
		(diff_agent a2 a1)
	)
)
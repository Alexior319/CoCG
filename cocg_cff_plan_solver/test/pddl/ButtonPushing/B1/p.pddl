(define (problem Button-1)
	(:domain Button-1)
	(:goal
		(and (Button_pushed b0) (Button_pushed b1) (Button_pushed b2))
	)
	(:init
		(agent_at a1 p1_1)
		(agent_at a2 p3_1)

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

		(green b0)
		(yellow b1)
		(green b2)

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
		(unknown 
			(Button_at b2 p3_1))
		(unknown
			(Button_pushed b2))
		(oneof
			(Button_at b2 p3_1)
			(Button_pushed b2))

		(diff_agent a1 a2)
		(diff_agent a2 a1)
	)
)
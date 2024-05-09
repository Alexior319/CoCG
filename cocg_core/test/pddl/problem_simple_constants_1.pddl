( define ( problem problem_1 )
( :domain simple )
( :objects
	m1 - message
	kitchen bedroom - room
)
( :init
	( robot_at leia kitchen )
	( person_at jack bedroom )
)
( :goal
	( and
		( robot_talk leia m1 jack )
	))
)

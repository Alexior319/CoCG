(define (problem hsd-2)
	(:domain hsd)
	(:goal
		(and
			(rescue p1)
			(rescue p2)
		)
	)
	(:init
		(in a1 loc3)
		(in a2 loc3)
		(in a3 loc3)
		(underneath p1 pillar1)
		(underneath p2 pillar2)

		(unknown
			(lying pillar1 loc3))
		(unknown
			(lying pillar1 loc2))
		(oneof
			(lying pillar1 loc3)
			(lying pillar1 loc2))
		(unknown
			(lying pillar2 loc3))
		(unknown
			(lying pillar2 loc1))
		(oneof
			(lying pillar2 loc3)
			(lying pillar2 loc1))
		(enable_patient p1 pillar1)

		(diff_agent a1 a2)
		(diff_agent a2 a1)

		(diff_agent a3 a1)
		(diff_agent a1 a3)
		(diff_agent a3 a2)
		(diff_agent a2 a3)
	)
)
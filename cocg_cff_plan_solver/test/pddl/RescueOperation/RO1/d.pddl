;;;; Heavy structural damage domain
(define (domain hsd)
	(:requirements :strips :typing :disjunctive-preconditions)
	(:types
		agent location pillar patient
	)

	(:predicates
		(in ?a - agent ?l - location)
		(rescue ?p - patient)
		(patient_at ?p - patient ?l - location)
		(lying ?p - pillar ?l - location)
		(diff_agent ?a1 - agent ?a1 - agent)
		(enable_patient ?pat - patient ?p - pillar)
		(underneath ?pat - patient ?p - pillar)
	)

	(:constants
		pillar1 pillar2 - pillar
		a1 a2 a3 - agent
		p1 p2 - patient
		loc1 loc2 loc3 - location
	)

	(:action move
		:parameters (?a - agent ?r1 - location ?r2 - location)
		:precondition (and (in ?a ?r1))
		:effect (and (not (in ?a ?r1)) (in ?a ?r2))
	)

	(:action observe_pillar
		:parameters (?a - agent ?p - pillar ?i - location)
		:precondition (and (in ?a ?i))
		:observe
		(lying ?p ?i)
	)

	(:action joint_lift
		:parameters (?a1 - agent ?a2 - agent ?p - pillar ?pat - patient ?r1 - location)
		:precondition (and
			(in ?a1 ?r1) (in ?a2 ?r1) (lying ?p ?r1) (underneath ?pat ?p) (diff_agent ?a1 ?a2))
		:effect (and
			(not (lying ?p ?r1)) (not (underneath ?pat ?p)) (enable_patient ?pat ?p))
	)

	(:action joint_rescue
		:parameters (?a1 - agent ?a2 - agent ?pat - patient ?p - pillar ?r1 - location)
		:precondition (and
			(in ?a1 ?r1)
			(in ?a2 ?r1)
			(diff_agent ?a1 ?a2)
			(enable_patient ?pat ?p))
		:effect (and (rescue ?pat))
	)

)
(define (domain blocksworld)
    (:requirements :strips :typing :disjunctive-preconditions)

    (:types
        block robot location
    )
    (:predicates
        (robot-at ?r - robot ?p - location)
        (hand-empty ?r - robot)
        (block-on-hand ?r - robot ?b - block)
        (block-at ?b - block ?p - location)
    )

    (:action locate-robot-at
        :parameters (?r - robot ?p - location)
        :observe
        (robot-at ?r ?p)
    )

    (:action sense-hand-empty
        :parameters (?r - robot)
        :observe
        (hand-empty ?r)
    )

    (:action sense-block-on-hand
        :parameters (?r - robot ?b - block)
        :observe
        (block-on-hand ?r ?b)
    )

    (:action sense-block-at
        :parameters (?r - robot ?b - block ?p - location)
        :precondition (and
            (robot-at ?r ?p)
        )
        :observe
        (block-at ?b ?p)
    )

    (:action move-to
        :parameters (?r - robot ?pf - location ?pt - location)
        :precondition (and (robot-at ?r ?pf) (hand-empty ?r))
        :effect (and (not (robot-at ?r ?pf)) (robot-at ?r ?pt))
    )

    (:action move-with-block
        :parameters (?r - robot ?b - block ?pf - location ?pt - location)
        :precondition (and (robot-at ?r ?pf) (block-on-hand ?r ?b))
        :effect (and (not (robot-at ?r ?pf)) (robot-at ?r ?pt))
    )

    (:action pickup
        :parameters (?r - robot ?b - block ?p - location)
        :precondition (and (robot-at ?r ?p) (block-at ?b ?p) (hand-empty ?r))
        :effect (and (not (block-at ?b ?p)) (not (hand-empty ?r)) (block-on-hand ?r ?b))
    )

    (:action place
        :parameters (?r - robot ?b - block ?p - location)
        :precondition (and (robot-at ?r ?p) (block-on-hand ?r ?b))
        :effect (and (not (block-on-hand ?r ?b)) (hand-empty ?r) (block-at ?b ?p))
    )

)